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
            if(m == nullptr) {
                std::cout << "Oh boy bad sphere" << std::endl;
            }
        }
        
        virtual RayIntersection checkIntersection(const Ray&) const override;
    };
    
    std::ostream& operator<<(std::ostream&, const Sphere&);
}
