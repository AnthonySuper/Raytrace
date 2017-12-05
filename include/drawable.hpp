#pragma once
#include <ray_intersection.hpp>
#include <ray_result.hpp>
#include <box.hpp>
#include <string>

namespace NM {
    class Drawable {
    public:
        
        virtual RayIntersection checkIntersection(const Ray&) const = 0;
        virtual std::string print();
        virtual Vec4 midpoint() const;
        virtual size_t complexity() const;
        virtual void expandToFit(Box&) const = 0;
        virtual bool intersects(RayResult& r) const;
        virtual void swapInfo(RayResult&) const = 0;
        virtual ~Drawable() = default;
    
    };
    
}
