#pragma once
#include <globals.hpp>
#include <drawable.hpp>
#include <vec4.hpp>
#include <ray_intersection.hpp>
#include <material.hpp>


namespace NM {
    class Sphere : public Drawable {
    public:
        FloatType radius;
        FloatType radiusSquared;
        Vec4 position;
        std::shared_ptr<Material> material;

        inline Sphere(FloatType r,
                                const Vec4& p,
                                std::shared_ptr<Material> m = nullptr) :
        radius(r), radiusSquared(r*r), position(p), material(m) {
        }
        
        virtual RayIntersection checkIntersection(const Ray&) const override final;
        virtual size_t complexity() const override final;
        virtual Vec4 midpoint() const override final;
        virtual bool intersects(const Box& b) const override final;
        virtual bool intersects(RayResult&) const override final;
        virtual void expandToFit(Box& b) const override final;
    };
    
    std::ostream& operator<<(std::ostream&, const Sphere&);
}
