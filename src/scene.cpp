#include <scene.hpp>
#include <iostream>

namespace NM {
    void Scene::addModel(const TransformedModel & mt) {
        models.emplace_back(mt);
    }
    
    void Scene::addModel(TransformedModel && mt) {
        models.emplace_back(mt);
    }
    
    Image Scene::render(Camera cam, size_t height, size_t width, bool progress) {
        std::vector<FloatType> dists;
        dists.resize(height * width, 0);
        Image toRet(height, width);
        std::atomic_size_t idx(0);
        const auto rays = cam.getRays(height, width);
        std::vector<std::thread> threadPool;
        for(int i = 0; i < std::thread::hardware_concurrency(); ++i) {
            threadPool.emplace_back([&]{
                while(true) {
                    auto ourIdx = ++idx;
                    if(ourIdx >= height * width) {
                        break;
                    }
                    auto& ourPixel = dists.at(ourIdx);
                    auto ourRay = rays.at(ourIdx);
                    auto intersect = getClosestIntersection(ourRay);
                    if(! intersect) {
                        ourPixel = -1;
                    }
                    else {
                        ourPixel = (intersect.point() - ourRay.position).magnitude();
                    }
                }
            });
        }
        if(progress) {
            while(idx < height * width) {
                std::cout << "\r[";
                std::cout << std::fixed;
                std::cout << std::setprecision(4);
                const int termLength = 60;
                const float percent = idx / (height * width * 1.0);
                const int toWrite = termLength * percent;
                for(int i = 0; i < termLength; i++) {
                    std::cout << (i < toWrite ? "=" : " ");
                }
                std::cout << "] (" << (percent*100) << "%)";
                std::cout.flush();
                std::this_thread::sleep_for(std::chrono::nanoseconds(100000000));
            }
        }
        for(auto& t : threadPool) {
            t.join();
        }
        double min = dists.at(0);
        double max = min;
        for(size_t i = 1; i < dists.size(); ++i) {
            double d = dists.at(i);
            if(d < 0) continue;
            if(min == 0) {
                min = d;
            }
            min = std::min(d, min);
            max = std::max(d, max);
        }
        std::cout << max << "," << min << std::endl;
        for(size_t i = 0; i < dists.size(); ++i) {
            auto& dist = dists.at(i);
            auto& out = toRet.at(i);
            if(dist <= 0) {
                out = {255, 255, 255, 0};
                continue;
            }
            double ratio = 2 * (dist - min) / (max - min);
            out[0] = std::max(0.0, 255*(1.0 - ratio));
            out[2] = std::max(0.0, 255*(ratio - 1.0));
            out[1] = 255 - out.b() - out.r();
            if(out[0] > 255 || out[1] > 255 || out[2] > 255) {
                throw std::runtime_error("FUCK OFF");
            }
        }
        return toRet;
    }
    
    RayIntersection Scene::getClosestIntersection(NM::Ray &ray) const {
        RayIntersection toRet;
        FloatType dist = std::numeric_limits<FloatType>::max();
        for(const auto& m : models) {
            auto r = m.checkIntersection(ray);
            if(! r) continue;
            auto thisDist = (r.point() - ray.position).magnitude();
            if(thisDist < dist) {
                toRet = r;
                dist = thisDist;
            }
        }
        return toRet;
    }
}
