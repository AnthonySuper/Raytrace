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
        Ray _originalRay;
        
    public:
        const Material *material;
        inline RayIntersection() :
        intersecting(false),
        _point{},
        dist(-1),
        _originalRay{{}, {1, 0, 0}},
        material{}
        {}
        
        inline RayIntersection(const Vec4& intersectionPoint,
                               const Vec4& sn,
                               const Ray& ray,
                               const Material* mat = nullptr) :
        intersecting(true),
        _point{intersectionPoint},
        _surfaceNormal{sn},
        dist{(intersectionPoint - ray.position).magnitude()},
        _originalRay{ray},
        material{mat}
        {}
        
        RayIntersection& operator=(const RayIntersection&) = default;
        
        inline operator bool() const {
            return intersecting;
        }
        
        inline const Vec4& point() const {
            checkValid();
            return _point;
        }
        
        inline const Vec4& surfaceNormal() const {
            checkValid();
            return _surfaceNormal;
        }
        
        inline Vec4 point() {
            checkValid();
            return _point;
        }
        
        inline void assignNormal(const Vec4& normal) {
            checkValid();
            _surfaceNormal = normal;
        }
        
        inline const Ray& originalRay() const {
            return _originalRay;
        }
        
        inline Ray originalRay() {
            return _originalRay;
        }
        
        inline  Vec4 surfaceNormal() {
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
        
        inline void checkValid() const {
            if(! intersecting) {
                throw InvalidIntersectionError();
            }
        }
        
        inline bool compareExchange(const RayIntersection & other) {
            if(! wouldExchange(other)) return false;
            *this = other;
            return true;
        }
        
        inline bool compareExchangeNoMaterialOrRay(const RayIntersection& other) {
            if(! wouldExchange(other)) return false;
            assignNoMaterialOrRay(other);
            return true;
        }
        
        inline void assignNoMaterialOrRay(const RayIntersection& other) {
            intersecting = other.intersecting;
            _point = other._point;
            _surfaceNormal = other._surfaceNormal;
            dist = other.dist;
        }
        
        inline bool wouldExchange(const RayIntersection & other) {
            if(! intersecting && other) {
                return true;
            }
            if(! other) {
                return false;
            }
            if(other.dist < dist) {
                return true;
            }
            return false;
        }
    };
}
