#pragma once

#include <ray.hpp>
#include <errors.hpp>
#include <material.hpp>

namespace NM {
    class RayIntersection {
    private:
        bool intersecting;
        Vec4 _point;
        Vec4 _surfaceNormal;
        FloatType dist;
        
    public:
        Material material;
        inline constexpr RayIntersection() :
        intersecting(false), _point{}, dist(-1), material{} {}
        
        inline RayIntersection(const Vec4& intersectionPoint,
                               const Vec4& sn,
                               const Ray& ray,
                               const Material& mat = {}) :
        intersecting(true),
        _point{intersectionPoint},
        _surfaceNormal{sn},
        dist{(intersectionPoint - ray.position).magnitude()},
        material{mat}
        {}
        
        RayIntersection& operator=(const RayIntersection&) = default;
        
     
        inline constexpr operator bool() const {
            return intersecting;
        }
        
        inline constexpr const Vec4& point() const {
            checkValid();
            return _point;
        }
        
        inline constexpr const Vec4& surfaceNormal() const {
            checkValid();
            return _surfaceNormal;
        }
        
        inline constexpr Vec4 point() {
            checkValid();
            return _point;
        }
        
        inline constexpr Vec4 surfaceNormal() {
            checkValid();
            return _surfaceNormal;
        }
        
        inline FloatType getDistance() {
            checkValid();
            return dist;
        }
        
        inline FloatType getDistanceOrNegative() {
            return dist;
        }
        
        inline FloatType distance(const Vec4& p) const {
            checkValid();
            return (_point - p).magnitude();
        }
        
        inline constexpr void checkValid() const {
            if(! intersecting) {
                throw InvalidIntersectionError();
            }
        }
        
        inline bool compareExchange(const RayIntersection & other) {
            if(! intersecting) {
                *this = other;
                return true;
            }
            if(! other) {
                // we're intersecting, they're not
                return false;
            }
            if(other.dist < dist) {
                *this = other;
                return true;
            }
            return false;
        }
        
    };
}
