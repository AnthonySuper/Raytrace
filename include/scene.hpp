#pragma once

#include <model.hpp>
#include <camera.hpp>
#include <image.hpp>
#include <transformed_model.hpp>
#include <atomic>
#include <thread>

namespace NM {
    class Scene {
    public:
        void addObject(const TransformedModel&);
        void addObject(TransformedModel&&);
        void addObject(const Sphere &);
        RayIntersection trace(const Ray& in) const;
        void render(Image& img, const Camera& camera) const;
        
    private:
        std::vector<TransformedModel> models;
        std::vector<Sphere> spheres;
    };
}
