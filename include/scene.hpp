#pragma once

#include <transformed_model.hpp>

namespace NM {
    class Scene {
    public:
        void addModel(const TransformedModel &);
        void addModel(TransformedModel&&);
    private:
        std::vector<TransformedModel> models;
    };
}
