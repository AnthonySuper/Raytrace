#pragma once
#include <globals.hpp>
#include <ray.hpp>
#include <material.hpp>
#include <limits>

namespace NM {
    class Drawable;
    
    /**
     * @brief Model the result of tracing a ray into a scene.
     */
    class RayResult {
    public:
        
        /**
         * Make an empty (non-colliding) result from a given ray.
         */
        RayResult(const Ray& r) : originalRay(r), invDir{
            1/r.direction[0],
            1/r.direction[1],
            1/r.direction[2]
        } {}
        
        /**
         * @brief The original ray we are using to do the actual intersections
         */
        Ray originalRay;
        
        Vec4 invDir;
        
        /**
         * @brief The surface normal of the intersected point
         */
        Vec4 surfaceNormal;
        /**
         * @brief How far away is the current "good" intersection?
         * By default, we intersect at infinity.
         * This is equivalent to no intersection taking place.
         */
        FloatType distance = std::numeric_limits<FloatType>::max();
        
        /**
         * @brief What, if anything, did we intersect with?
         */
        const Drawable* drawable = nullptr;
        
        /**
         * @brief What material did we hit?
         */
        Material material;

        /**
         * @brief will this ray be refracted in any way?
         */
        bool isRefracted = false;
        
        FloatType eta = 1.0;
        
        bool swapDistance(double newDist, const Drawable *dr);
        
        inline bool betterDistance(double newDist) const {
            return (newDist > 0 && newDist < distance);
        }
        
        /**
         * @brief Returns the point of intersection in 3D space.
         */
        inline Vec4 point() const {
            return originalRay.position + (distance * originalRay.direction);
        }
        
        /**
         * @brief A simple way to ask if we had a real intersection
         */
        inline operator bool() const {
            return distance != std::numeric_limits<FloatType>::max();
        }
    };
}
