#pragma once
#include <globals.hpp>
#include <ray.hpp>
#include <material.hpp>
#include <limits>

namespace NM {
    class Drawable;
    
    class RayResult {
    public:
        
        RayResult(const Ray& r) : originalRay(r), invDir{
            1/r.direction[0],
            1/r.direction[1],
            1/r.direction[2]
        } {}
        
        /**
         * The original ray we are using to do the actual intersections
         */
        Ray originalRay;
        
        Vec4 invDir;
        
        /**
         * The surface normal of the intersected point
         */
        Vec4 surfaceNormal;
        /**
         * How far away is the current "good" intersection?
         */
        FloatType distance = std::numeric_limits<FloatType>::max();
        
        const Drawable* drawable = nullptr;
        
        Material material;

        bool isRefracted = false;

        FloatType eta = 1.0;
        
        bool swapDistance(double newDist, const Drawable *dr);
        
        inline bool betterDistance(double newDist) const {
            return (newDist > 0 && newDist < distance);
        }
        
        inline Vec4 point() const {
            return originalRay.position + (distance * originalRay.direction);
        }
        
        inline operator bool() const {
            return distance != std::numeric_limits<FloatType>::max();
        }
    };
}
