#pragma once

#include <model.hpp>
#include <camera.hpp>
#include <image.hpp>
#include <transformed_drawable.hpp>
#include <light.hpp>
#include <atomic>
#include <thread>

namespace NM {
    class Scene {
    public:
        Vec4 ambient;
        void addObject(const TransformedDrawable&);
        void addObject(TransformedDrawable&&);
        void addObject(const Sphere &);
        void addObject(const Light &);
        RayIntersection traceIntersection(const Ray& in) const;
        Vec4 colorize(const RayIntersection& ri) const;
        void render(Image& img, const Camera& camera) const;
        
        
    private:
        std::vector<TransformedDrawable> drawables;
        std::vector<Sphere> spheres;
        std::vector<Light> lights;
        friend std::ostream& operator<<(std::ostream&, const Scene&);
    };
    
    std::ostream& operator<<(std::ostream&, const Scene&);
}
