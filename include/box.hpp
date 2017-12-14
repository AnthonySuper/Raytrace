#pragma once
#include <vec4.hpp>
#include <ray_result.hpp>
#include <triangle.hpp>
#include <utility>



namespace NM {
    /**
     * @brief represent an Axis-Aligned Bounding Box in 3D space.
     */
    struct Box {
        constexpr static FloatType FloatMin = std::numeric_limits<FloatType>::lowest();
        constexpr static FloatType FloatMax = std::numeric_limits<FloatType>::max();
        /**
         * @brief The minimum axis bounds of the box.
         
         * By default, this is set to the maximum possible FloatType values.
         * This allows boxes to be constructed in a null state, then expanded to fit
         * points later on.
         */
        Vec4 min = {
            FloatMax,
            FloatMax,
            FloatMax
        };
        
        /**
         * @brief The maximum axis bounds in the box.
         *
         * By default, this is set to to the minimum possible FloatType values.
         * This means that boxes can be constructed in a null state, then expanded to fit
         * points or other objects later on, and still have the proper behavior.
         */
        Vec4 max = {
            FloatMin,
            FloatMin,
            FloatMin
        };
        
        /**
         * @breif Construct an undefined box
         * Doing collision tests on this box will give nonsense results.
         * However, calling any expandToFit functions will work properly!
         */
        inline constexpr Box() = default;
        /**
         * @brief Create a box with given min and max points.
         */
        inline constexpr Box(const Vec4& _min, const Vec4& _max) :
        min{_min}, max{_max} {}
        
        /**
         * @brief Construct a box from a triangle.
         */
        Box(const Triangle&);
        
        /**
         * A tuple type representing the minimum and maximum possible intersect distances.
         * A ray through a box will hit two sides of it.
         * There are cases were it is desirable to know the distances to both sides.
         */
        using MinMax = std::pair<FloatType, FloatType>;
        
        /**
         * Expands this box to fit a given point inside.
         */
        void expandToFit(const Vec4& other);
        /**
         * Expands this box to fit a given triangle inside.
         */
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

        /**
         * Get the minimum distance intersection for this box.
         * Does not count *negative* intersections.
         */
        inline FloatType intersectMinDist(const Ray& r, const Vec4& invDir) const {
            auto t = intersectDist(r, invDir);
            return t.first > 0 ? t.first : t.second;
        }
        
        /**
         * Get the minimum distance intersection for this box, or infinity if
         * there is no such positive intersection distance.
         */
        inline FloatType intersectOrInf(const Ray& r, const Vec4& invDir) const {
            auto t = intersectDist(r, invDir);
            FloatType ret = t.first < 0 ? t.second : t.first;
            return (ret < 0) ? std::numeric_limits<FloatType>::max() : ret;
        }
        
        /**
         * Get the intersection distances for this box, possibly including
         * one or more negative values.
         *
         * Note that this is not super reliable at the moment, as it uses the sentinel
         * value {-1, -1} to represent "no intersection," which is ad-hoc and bad coding.
         * I will hopefully fix that eventually.
         */
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
