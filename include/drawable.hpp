#pragma once
#include <ray_intersection.hpp>
#include <string>

namespace NM {
    class Drawable {
    public:
        virtual RayIntersection checkIntersection(const Ray&) const = 0;
        virtual std::string print();
        virtual ~Drawable() = default;
    };
    
}
