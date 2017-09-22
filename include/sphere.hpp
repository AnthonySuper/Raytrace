#pragma once
#include <globals.hpp>
#include <vec4.hpp>
#include <ray_intersection.hpp>


namespace NM {
    class Sphere {
    public:
        FloatType radius;
        FloatType radiusSquared;
        Vec4 position;
        constexpr inline Sphere(FloatType r, const Vec4& p) :
        radius(r), radiusSquared(r*r), position(p) {}
        
        RayIntersection checkIntersection(const Ray&) const;
    };
}
