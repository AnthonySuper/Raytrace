#include <scene.hpp>
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
    
    Vec4 Scene::colorize(const RayIntersection &ri) const {
        if(! ri) return {};
        const Material& mtl = ri.material;
        Vec4 color = ambient.pairwiseProduct(mtl.ambient);
        
        for(auto& light : lights) {
            Vec4 toLight = (light.position - ri.point()).toUnit();
            FloatType dotProduct = ri.surfaceNormal().dot(toLight);
            if(dotProduct > 0.0) {
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
        return color;
    }
    
    void Scene::render(NM::Image &img, const NM::Camera &camera) const {
        using namespace std::chrono_literals;
        const auto rays = camera.getRays(img.height, img.width);
        size_t raysSize = rays.size();
        auto& pixels = img.getPixels();
        std::atomic_size_t idx(0);
        std::vector<std::thread> workThreads;
        for(int i = 0; i < std::thread::hardware_concurrency(); ++i) {
            workThreads.emplace_back([&]() {
                while(true) {
                    size_t ourIdx = idx++;
                    if(ourIdx >= raysSize) return;
                    auto ourRay = rays[ourIdx];
                    RayIntersection it = traceIntersection(ourRay);
                    pixels.at(ourIdx) = colorize(it);
                }
            });
        }
        for(auto& t: workThreads) {
            t.join();
        }
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
