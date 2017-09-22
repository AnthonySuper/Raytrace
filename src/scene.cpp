#include <scene.hpp>

namespace NM {
    void Scene::addModel(const TransformedModel & mt) {
        models.emplace_back(mt);
    }
    
    void Scene::addModel(TransformedModel && mt) {
        models.emplace_back(mt);
    }
}
