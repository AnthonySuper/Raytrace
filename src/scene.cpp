#include <scene.hpp>
#include <globals.hpp>
#include <chrono>

namespace NM {
    void Scene::addObject(const NM::TransformedDrawable & tm) {
        drawables.emplace_back(tm);
    }
    
    void Scene::addObject(TransformedDrawable && tm) {
        drawables.emplace_back(std::forward<TransformedDrawable>(tm));
    }
    
    void Scene::addObject(const NM::Sphere & s) {
        spheres.emplace_back(s);
    }
    
    void Scene::addObject(const Light& l) {
        lights.emplace_back(l);
    }
    
    RayIntersection Scene::traceIntersection(const Ray& in) const {
        RayIntersection toRet;
        for(const auto &s: drawables) {
            toRet.compareExchange(s.checkIntersection(in));
        }
        for(const auto &s: spheres) {
            toRet.compareExchange(s.checkIntersection(in));
        }
        return toRet;
    }
    
    void Scene::colorize(const RayIntersection &ri,
                         Vec4& accum,
                         const Vec4& refAt,
                         unsigned int depth) const {
        
        if(! ri) return;
        if(ri.material == nullptr) {
            throw std::runtime_error("Not allowed!");
        }
        const Material& mtl = ri.material ? *ri.material : Material{};
        Vec4 color = ambient.pairwiseProduct(mtl.ambient);
        for(auto& light : lights) {
            Vec4 toLight = (light.position - ri.point()).toUnit();
            FloatType dotProduct = ri.surfaceNormal().dot(toLight);
            if(dotProduct > 0.0 && ! traceIntersection({ri.point(), toLight})) {
                Vec4 diff = mtl.diffuse.pairwiseProduct(light.color);
                color += (dotProduct * diff);
                Vec4 toC = (ri.originalRay().position -
                            ri.point()).toUnit();
                FloatType twice = (2 * ri.surfaceNormal().dot(toLight));
                Vec4 spr = (twice * ri.surfaceNormal()) - toLight;
                FloatType expon = std::pow(toC.dot(spr), mtl.specularExpon);
                Vec4 modSpec = mtl.specular.pairwiseProduct(light.color);
                color += (modSpec * expon);
            }
        }
        accum += refAt.pairwiseProduct(color) * (1.0 - mtl.reflectivity);
        if(depth > 0) {
            // Time to recurse
            Vec4 toCamera = -1 * ri.originalRay().direction;
            const Vec4& n = ri.surfaceNormal();
            Vec4 reflect = ((2 * n.dot(toCamera)) * n) - toCamera;
            RayIntersection ri2 = traceIntersection({ri.point(), reflect.toUnit()});
            Vec4 newRefAt = mtl.attunation.pairwiseProduct(refAt) * mtl.reflectivity;
            colorize(ri2,
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
        for(int i = 0; i < getConcurrency(); ++i) {
            workThreads.emplace_back([&]() {
                while(true) {
                    size_t ourIdx = idx++;
                    if(ourIdx >= raysSize) return;
                    auto ourRay = rays[ourIdx];
                    RayIntersection it = traceIntersection(ourRay);
                    if(ourIdx > 7*16 + 8) {
                        // break here
                    }
                    colorize(it,
                             pixels.at(ourIdx),
                             {1, 1, 1},
                             recursionDepth);
                }
            });
        }
        std::thread progressBar([&]() {
            while(true) {
                size_t ourIdx = idx;
                if(ourIdx > raysSize) return;
                outputProgress("Tracing progress", ourIdx, raysSize);
                std::this_thread::sleep_for(
                    std::chrono::nanoseconds(250)
                );

            }
        });
        for(auto& t: workThreads) {
            t.join();
        }
        progressBar.join();
    }
    
    size_t Scene::getConcurrency() const {
        return std::thread::hardware_concurrency();
    }
    
    std::ostream& operator<<(std::ostream& os, const Scene& s) {
        using std::endl;
        os << "Scene:" << endl;
        os << "\t Lights:" << endl;
        for(auto& l : s.lights) {
            os << "\t\t" << l << endl;
        }
        os << "\t Drawables:" << endl;
        for(auto& d: s.drawables) {
            os << "\t\t" << d << endl;
        }
        os << "\t Spheres:" << endl;
        for(auto& sp : s.spheres) {
            os << "\t\t" << sp << endl;
        }
        return os;
    }
}
