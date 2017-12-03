#pragma once
#include <vec4.hpp>
#include <ray_result.hpp>
#include <triangle.hpp>
#include <utility>



namespace NM {
    struct Box {
        constexpr static FloatType FloatMin = std::numeric_limits<FloatType>::lowest();
        constexpr static FloatType  FloatMax = std::numeric_limits<FloatType>::max();
        Vec4 min = {
            FloatMax,
            FloatMax,
            FloatMax
        };
        Vec4 max = {
            FloatMin,
            FloatMin,
            FloatMin
        };

        inline constexpr Box() = default;
        inline constexpr Box(const Vec4& _min, const Vec4& _max) :
        min{_min}, max{_max} {}
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

        inline FloatType intersectMinDist(const Ray& r, const Vec4& invDir) const {
            auto t = intersectDist(r, invDir);
            return t.first > 0 ? t.first : t.second;
        }
        
        inline FloatType intersectOrInf(const Ray& r, const Vec4& invDir) const {
            auto t = intersectDist(r, invDir);
            FloatType ret = t.first < 0 ? t.second : t.first;
            return (ret < 0) ? std::numeric_limits<FloatType>::max() : ret;
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
        
        FloatType surfaceArea() const;
    };
    
    std::ostream& operator<<(std::ostream&, const Box&);
    
}
