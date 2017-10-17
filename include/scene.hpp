#pragma once

#include <model.hpp>
#include <camera.hpp>
#include <image.hpp>
#include <transformed_drawable.hpp>
#include <atomic>
#include <thread>

namespace NM {
    class Scene {
    public:
        void addObject(const TransformedDrawable&);
        void addObject(TransformedDrawable&&);
        void addObject(const Sphere &);
        RayIntersection trace(const Ray& in) const;
        void render(Image& img, const Camera& camera) const;
        
    private:
        std::vector<TransformedDrawable> drawables;
        std::vector<Sphere> spheres;
    };
}
