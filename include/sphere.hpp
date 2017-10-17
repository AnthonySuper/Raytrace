#pragma once
#include <globals.hpp>
#include <drawable.hpp>
#include <vec4.hpp>
#include <ray_intersection.hpp>


namespace NM {
    class Sphere : public Drawable {
    public:

        FloatType radius;
        FloatType radiusSquared;
        Vec4 position;

        constexpr inline Sphere(FloatType r, const Vec4& p) :
        radius(r), radiusSquared(r*r), position(p) {}
        
        virtual RayIntersection checkIntersection(const Ray&) const override;
    };
}
