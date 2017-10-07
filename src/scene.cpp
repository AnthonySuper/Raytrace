#include <scene.hpp>
#include <chrono>

namespace NM {
    void Scene::addObject(const NM::TransformedModel & tm) {
        models.emplace_back(tm);
    }
    
    void Scene::addObject(NM::TransformedModel && tm) {
        models.emplace_back(tm);
    }
    
    void Scene::addObject(const NM::Sphere & s) {
        spheres.emplace_back(s);
    }
    
    RayIntersection Scene::trace(const Ray& in) const {
        RayIntersection toRet;
        for(const auto &s: models) {
            toRet.compareExchange(s.checkIntersection(in));
        }
        for(const auto &s: spheres) {
            toRet.compareExchange(s.checkIntersection(in));
        }
        return toRet;
    }
    
    void Scene::render(NM::Image &img, const NM::Camera &camera) const {
        using namespace std::chrono_literals;
        const auto& rays = camera.getRays(img.height, img.width);
        size_t raysSize = rays.size();
        std::vector<FloatType> dists;
        dists.resize(raysSize);
        std::atomic_size_t idx(0);
        std::vector<std::thread> workThreads;
        for(int i = 0; i < std::thread::hardware_concurrency(); ++i) {
            workThreads.emplace_back([&]() {
                while(true) {
                    size_t ourIdx = idx++;
                    if(ourIdx >= raysSize) return;
                    auto ourRay = rays[ourIdx];
                    RayIntersection it = trace(ourRay);
                    dists[ourIdx] = it.getDistanceOrNegative();
                }
            });
        }
        std::cout << std::endl;
        std::cout.precision(3);
        std::cout << std::fixed;
        while(true) {
            double ratio = idx / static_cast<double>(raysSize);     
            std::cout << (ratio * 100) << "% done...";
            std::cout << std::endl;
            if(idx >= raysSize) break;
            std::this_thread::sleep_for(1s);

        }
        for(auto& t: workThreads) {
            t.join();
        }
        double tmax = 0;
        double tmin = std::numeric_limits<FloatType>::max();
        for(FloatType d: dists) {
            if(d < 0) continue;
            tmax = std::max(tmax, d);
            tmin = std::min(tmin, d);
        }

        for(size_t i = 0; i < raysSize; ++i) {
            FloatType t = dists.at(i);
            if(t < 0) {
                img.getPixels().at(i) = {
                    239,
                    239,
                    239
                };
                continue;
            }
            FloatType ratio = 2 * (t - tmin) / (tmax - tmin);
            FloatType r = std::max(0.0, 255 * (1 - ratio));
            FloatType b = std::max(0.0, 255 *(ratio - 1));
            FloatType g = 255 - (b + r);
            img.getPixels().at(i) = {
                r,
                g,
                b
            };
        }
    }
}
