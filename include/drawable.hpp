#pragma once
#include <ray_intersection.hpp>

namespace NM {
    class Drawable {
    public:
        virtual RayIntersection checkIntersection(const Ray&) const = 0;
        virtual ~Drawable() = default;
    };
    
}
