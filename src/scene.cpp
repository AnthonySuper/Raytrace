#include <scene.hpp>

namespace NM {
    
    void Scene::addModel(const NM::Model &m) {
        models.emplace_back(m);
    }
    
    void Scene::addModel(NM::Model && m) {
        models.emplace_back(m);
    }
}
