#include <scene.hpp>
#include <globals.hpp>
#include <chrono>



namespace NM {

    void Scene::addObject(const NM::Sphere & s) {
        spheres.emplace_back(s);
    }

    void Scene::addObject(const Light& l) {
        lights.emplace_back(l);
    }
    
    void Scene::finalize() {
        size_t i = 0;
        bonsai.reset();
        for(auto& sphere: spheres) {
            bonsai.add(&sphere);
        }
        for(auto& f: faces) {
            bonsai.add(&f);
        }
        bonsai.expandBox();
        bonsai.partition(getConcurrency());
    }

    RayIntersection Scene::traceIntersection(const Ray& in) const {
        RayIntersection toRet;
        return toRet;
    }
    
    void Scene::intersect(RayResult &r) const {
        bonsai.intersectStack(r);
    }

    void Scene::colorize(const RayResult &ri,
            Vec4& accum,
            const Vec4& refAt,
            unsigned int depth) const {
        if(! ri) return;
        Vec4 toC = (ri.originalRay.position - ri.point()).toUnit();
        Vec4 normal = ri.surfaceNormal;
        const Material& mtl = ri.material ? *ri.material : Material{};
        Vec4 color = ambient.pairwiseProduct(mtl.ambient);
        for(auto& light : lights) {
            Vec4 toLight = (light.position - ri.point());
            Vec4 toLightUnit = toLight.toUnit();
            FloatType dot = normal.dot(toLightUnit);
            if(dot < 0.0) continue;
            // Test to see if there's something in the way.
            const auto DoubleCollideFixer = 0.01;
            Ray lightRay{
                ri.point() + (DoubleCollideFixer * toLightUnit),
                toLightUnit
            };
            RayResult res{lightRay};
            res.distance = toLight.magnitude();
            intersect(res);
            // This light did nothing due to a shadow, rip in peace
            if(res && res.distance < toLight.magnitude()) continue;
            Vec4 diff = mtl.diffuse.pairwiseProduct(light.color);
            color += (diff * dot);
            FloatType twice = 2 * normal.dot(toLightUnit);
            if(twice < 0) continue;
            Vec4 spr = (twice * normal) - toLightUnit;
            FloatType cspr = toC.dot(spr);
                
            if(cspr < 0) continue;
            FloatType expon = std::pow(cspr, mtl.specularExpon);
            Vec4 modSpec = mtl.specular.pairwiseProduct(light.color);
            color += (modSpec * expon);
        }
        accum += refAt.pairwiseProduct(color);
        if(depth > 0) {
            // Time to recurse
            Vec4 toCamera = -1 * ri.originalRay.direction;
            const Vec4& n = normal;
            Vec4 reflect = ((2 * n.dot(toCamera)) * n) - toCamera;
            RayResult r2 = {{ri.point(), reflect.toUnit()}};
            intersect(r2);
            Vec4 newRefAt = mtl.attunation.pairwiseProduct(refAt);
            colorize(r2,
                    accum,
                    newRefAt,
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
