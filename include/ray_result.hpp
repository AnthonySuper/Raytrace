#pragma once
#include <globals.hpp>
#include <ray.hpp>
#include <material.hpp>

namespace NM {
    class RayResult {
    public:
        /**
         * The original ray we are using to do the actual intersections
         */
        Ray originalRay;
        /**
         * How far away is the current "good" intersection?
         */
        double distance = -1.0;
        /**
         * What is the material of the current "good" intersection?
         */
        const Material* material = nullptr;
        
        void swapDistance(double newDist, const Material* mtlPtr);
        
        inline bool betterDistance(double newDist) const {
            return distance < 0 || newDist < distance;
        }
    };
}
