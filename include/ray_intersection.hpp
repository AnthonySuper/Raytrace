#pragma once

#include <ray.hpp>
#include <errors.hpp>

namespace NM {
    class RayIntersection {
    private:
        bool intersecting;
        Vec4 _point;
        Vec4 _surfaceNormal;
        FloatType dist;
    public:
        inline constexpr RayIntersection() :
        intersecting(false), _point{}, dist(-1) {}
        
        inline RayIntersection(const Vec4& intersectionPoint,
                               const Vec4& sn,
                               const Ray& ray) :
        intersecting(true),
        _point{intersectionPoint},
        _surfaceNormal{sn},
        dist{(intersectionPoint - ray.position).magnitude()}
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
        
        void compareExchange(const RayIntersection & other);
        
    };
}
