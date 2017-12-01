#pragma once
#include <vec4.hpp>
#include <ray_result.hpp>
#include <triangle.hpp>
#include <utility>



namespace NM {
    struct Box {
        Vec4 min;
        Vec4 max;
        Vec4 extent;
        
        inline constexpr Box(const Vec4& _min, const Vec4& _max) :
        min{_min}, max{_max}, extent{max - min} {}
        Box(const Triangle&);
        
        using MinMax = std::pair<FloatType, FloatType>;
        
        void expandToFit(const Vec4& other);
        void expandToFit(const Triangle& other);
        /**
         * Will this ray intersect this box?
         */
        inline bool intersect(NM::RayResult & rr) const {
            auto res = intersectDist(rr.originalRay,
                                     rr.invDir);
            auto tmin = res.first;
            auto tmax = res.second;
        
            return (rr.distance < 0 && tmax > 0) ||
                (tmin < rr.distance && tmax > 0);
        }
        
        inline MinMax intersectDist(const Ray& r, const Vec4& invDir) const {
            using std::swap;
            auto tmin = (min.x() - r.position.x()) * invDir.x();
            auto tmax = (max.x() - r.position.x()) * invDir.x();
            if(tmin > tmax) swap(tmin, tmax);
            
            auto tymin = (min.y() - r.position.y()) * invDir.y();
            auto tymax = (max.y() - r.position.y()) * invDir.y();
            
            if(tymin > tymax) swap(tymin, tymax);
            
            if(tmin > tymax || tymin > tmax) {
                return {-1, -1};
            }
            
            if(tymin > tmin) {
                tmin = tymin;
            }
            if(tymax < tmax) {
                tmax = tymax;
            }
            
            auto tzmin = (min.z() - r.position.z()) * invDir.z();
            auto tzmax = (max.z() - r.position.z()) * invDir.z();
            
            if(tzmin > tzmax) swap(tzmin, tzmax);
            
            if(tmin > tzmax || tzmin > tmax) {
                return {-1, -1};
            }
            if(tzmin > tmin) {
                tmin = tzmin;
            }
            if(tzmax < tmax) {
                tmax = tzmax;
            }
            if(tmin < 0 && tmax < 0) return {-1, -1};
            return {tmin, tmax};
        }
    };
}
