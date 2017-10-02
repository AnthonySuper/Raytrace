#pragma once

#include <model.hpp>
#include <transformed_model.hpp>

namespace NM {
    class Scene {
    public:
        void addObject(const TransformedModel&);
        void addObject(TransformedModel&&);
        void addObject(const Sphere &);
        RayIntersection trace(const Ray& in) const;
        
    private:
        std::vector<TransformedModel> models;
        std::vector<Sphere> spheres;
    };
}
