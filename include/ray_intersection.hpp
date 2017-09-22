#pragma once

#include <ray.hpp>
#include <errors.hpp>

namespace NM {
    class RayIntersection {
    private:
        bool intersecting;
        Vec4 _point;
        Vec4 _surfaceNormal;
    public:
        inline constexpr RayIntersection() :
        intersecting(false), _point{} {}
        
        inline constexpr RayIntersection(const Vec4& intersectionPoint,
                                         const Vec4& sn) :
        intersecting(true),
        _point{intersectionPoint},
        _surfaceNormal{sn} {}
        
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
        
        inline FloatType distance(const Vec4& p) const {
            checkValid();
            return (_point - p).magnitude();
        }
        
        inline constexpr void checkValid() const {
            if(! intersecting) {
                throw InvalidIntersectionError();
            }
        }
        
    };
}
