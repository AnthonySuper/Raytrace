#include <scene.hpp>
#include <globals.hpp>
#include <chrono>
#include <timer.hpp>


namespace NM {

    void Scene::addObject(const NM::Sphere & s) {
        spheres.emplace_back(s);
    }

    void Scene::addObject(const Light& l) {
        lights.emplace_back(l);
    }
    
    void Scene::finalize() {
        auto start = std::chrono::system_clock::now();
        std::cout << "Starting scene finalize.";
        bonsai.reset();
        for(auto& sphere: spheres) {
            bonsai.add(&sphere);
        }
        Timer::report(
                std::string("Added ") + 
                std::to_string(spheres.size()) + 
                std::string(" spheres")
        );
        for(auto& f: faces) {
            bonsai.add(&f);
        }
        Timer::report(
                std::string("Added ") + 
                std::to_string(faces.size()) +
                std::string(" faces")
        );
        bonsai.expandBox();
        std::cout << "Starting BVH construction (may take a bit)" << std::endl;
        bonsai.partition(getConcurrency());
        Timer::report("Constructed BVH");
    }

    RayIntersection Scene::traceIntersection(const Ray& in) const {
        RayIntersection toRet;
        return toRet;
    }
    
    void Scene::intersect(RayResult &r) const {
        bonsai.intersectStack(r);
    }
    
    Vec4 Scene::colorRay(Vec4 point,
                         Vec4 normal,
                         Vec4 toC,
                         const Light& light,
                         const Material& mtl) const {
        Vec4 color{0, 0, 0};
        Vec4 toLight = (light.position - point);
        Vec4 toLightUnit = toLight.toUnit();
        FloatType dot = normal.dot(toLightUnit);
        if(dot < 0.0) return color;
        // Test to see if there's something in the way.
        const auto DoubleCollideFixer = 0.01;
        Ray lightRay{
            point + (DoubleCollideFixer * toLightUnit),
            toLightUnit
        };
        RayResult res{lightRay};
        res.distance = toLight.magnitude();
        intersect(res);
        // This light did nothing due to a shadow, rip in peace
        if(res && res.distance < toLight.magnitude()) return color;
        Vec4 diff = mtl.diffuse.pairwiseProduct(light.color);
        color += (diff * dot);
        FloatType twice = 2 * normal.dot(toLightUnit);
        if(twice < 0) return color;
        Vec4 spr = (twice * normal) - toLightUnit;
        FloatType cspr = toC.dot(spr);
        if(cspr < 0) return color;
        FloatType expon = std::pow(cspr, mtl.specularExpon);
        Vec4 modSpec = mtl.specular.pairwiseProduct(light.color);
        color += (modSpec * expon);
        return color;
    }

    Vec4 Scene::refractDirection(Vec4 w,
            Vec4 pt,
            Vec4 n,
            FloatType eta_in,
            FloatType eta_out) const {
        auto etar = eta_in / eta_out;
        auto a = -etar;
        auto wn = w.dot(n);
        auto radsq = (etar * etar) * (wn * wn - 1.0) + 1.0;
        if(radsq < 0.0) {
            // no refraction I guess lmao
            return {0, 0, 0};
        }
        auto b = (etar * wn) - std::sqrt(radsq);
        auto t = (a * w) + (b * n);
        return t.toUnit();
    }

    void Scene::refractRay(RayResult &ri,
            Vec4& color,
            size_t depth) const {
        if(depth == 0 || ! ri) return;
        Vec4 t1 = refractDirection(- ri.originalRay.direction,
                ri.point(),
                ri.surfaceNormal,
                ri.eta,
                ri.material.eta);
        if(t1 == Vec4{0.0, 0.0, 0.0}) return;
        const Sphere* sp = dynamic_cast<const Sphere*>(ri.drawable);
        if(sp == nullptr || sp->material == nullptr) return;
        Vec4 pointOut = sp->refractExit({ri.point(), t1});
        Vec4 nin = (sp->position - pointOut).toUnit();
        Vec4 outDir = refractDirection(- t1,
                pointOut,
                nin,
                ri.material.eta,
                ri.eta);
        RayResult rr({pointOut, outDir.toUnit()});
        intersect(rr);
        Vec4 outColor;
        colorize(rr,
                 outColor,
                 {1, 1, 1},
                 depth - 1);
        color += (outColor.pairwiseProduct(Vec4{1.0, 1.0, 1.0} - ri.material.opacity));
    }

    void Scene::colorize(RayResult &ri,
            Vec4& accum,
            const Vec4& refAt,
            unsigned int depth) const {
        if(! ri) return;
        ri.drawable->swapInfo(ri);
        Vec4 toC = (ri.originalRay.position - ri.point()).toUnit();
        Vec4 normal = ri.surfaceNormal;
        const Material& mtl = ri.material;
        Vec4 color = ambient.pairwiseProduct(mtl.ambient);
        for(auto& light : lights) {
            color += colorRay(ri.point(),
                              normal,
                              toC,
                              light,
                              mtl);
        }
        accum += refAt.pairwiseProduct(color).pairwiseProduct(mtl.opacity);
        auto newRef = mtl.attunation.pairwiseProduct(refAt)
            .pairwiseProduct(mtl.opacity);
        if(depth > 0 && newRef.magnitude() > 0.1) {
            // Time to recurse
            Vec4 toCamera = -1 * ri.originalRay.direction;
            const Vec4& n = normal;
            Vec4 reflect = ((2 * n.dot(toCamera)) * n) - toCamera;
            RayResult r2 = {{ri.point(), reflect.toUnit()}};
            intersect(r2);
            colorize(r2,
                    accum,
                    newRef,
                    depth - 1);
        }
        if(depth > 0 && mtl.opacity != Vec4{1.0, 1.0, 1.0}) {
            refractRay(ri,
                    accum,
                    depth - 1);
        }
    }

    void Scene::render(NM::Image &img, const NM::Camera &camera) const {
        using namespace std::chrono_literals;
        const auto rays = camera.rayGeneratorFor(img);
        size_t raysSize = rays.size();
        auto& pixels = img.getPixels();
        std::atomic_size_t idx(0);
        std::vector<std::thread> workThreads;
        auto conc = getConcurrency();
        for(int i = 0; i < conc; ++i) {
            workThreads.emplace_back([&]() {
                    while(true) {
                        size_t ourIdx = idx++;
                    if(ourIdx >= raysSize) return;
                    auto ourRay = rays[ourIdx];
                    RayResult ourResult{ourRay};
                    intersect(ourResult);
                    Vec4& ourPixel = pixels.at(ourIdx);
                    colorize(ourResult,
                            ourPixel,
                            {1, 1, 1},
                            recursionDepth);
                    ourPixel.positiveize();  
                    }});
        }

        std::thread progressBar([&]() {
                while(shouldLog) {
                size_t ourIdx = idx;
                if(ourIdx > raysSize) return;
                outputProgress("Tracing progress", ourIdx, raysSize);
                std::cout << "\t Savings: " << raySavings();
                std::cout.flush();
                std::this_thread::sleep_for(
                        std::chrono::milliseconds(250)
                        );

                }
                });
        for(auto& t: workThreads) {
            t.join();
        }
        progressBar.join();
        std::cout << std::endl;
        auto t = bonsai.getIntersects();
        std::cout << "We performed " << t.first << " intersections " << std::endl;
        std::cout << "Without our BVH, we needed " << t.second << " intersections. ";
        auto f = static_cast<FloatType>(t.second);
        std::cout << std::endl;
        std::cout << ((t.first) / f)*100 << "% intersections actually performed";
        std::cout << std::endl;
    }

    size_t Scene::getConcurrency() const {
        if(concurrent) {
            return std::thread::hardware_concurrency();
        }
        return 1;
    }

    std::ostream& operator<<(std::ostream& os, const Scene& s) {
        using std::endl;
        os << "Scene:" << endl;
        os << "\t Lights:" << endl;
        for(auto& l : s.lights) {
            os << "\t\t" << l << endl;
        }
        os << "\t Drawables:" << endl;
        os << "\t Spheres:" << endl;
        for(auto& sp : s.spheres) {
            os << "\t\t" << sp << endl;
        }
        return os;
    }
}
