#include <scene.hpp>

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
}
