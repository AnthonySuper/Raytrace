#pragma once

#include <model.hpp>

namespace NM {
    class Scene {
    public:
        void addModel(const Model&);
        void addModel(Model&&);
    private:
        std::vector<Model> models;
    };
}
