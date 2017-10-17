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
        Material material;

        constexpr inline Sphere(FloatType r,
                                const Vec4& p,
                                const Material &m = {}) :
        radius(r), radiusSquared(r*r), position(p), material(m) {}
        
        virtual RayIntersection checkIntersection(const Ray&) const override;
    };
    
    std::ostream& operator<<(std::ostream&, const Sphere&);
}
