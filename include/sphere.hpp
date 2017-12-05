#pragma once
#include <globals.hpp>
#include <drawable.hpp>
#include <vec4.hpp>
#include <ray_intersection.hpp>
#include <material.hpp>


namespace NM {
    class Sphere : public Drawable {
    public:
        FloatType radius;
        FloatType radiusSquared;
        Vec4 position;
        std::shared_ptr<Material> material;

        inline Sphere(FloatType r,
                                const Vec4& p,
                                std::shared_ptr<Material> m = nullptr) :
        radius(r), radiusSquared(r*r), position(p), material(m) {
        }
        
        virtual RayIntersection checkIntersection(const Ray&) const override final;
        virtual size_t complexity() const override final;
        virtual Vec4 midpoint() const override final;
        virtual void swapInfo(RayResult&) const override final;
        
        inline bool intersectsInline(RayResult &r) const {
            const auto& ray = r.originalRay;
            auto cVec = (position - ray.position);
            auto vMag = cVec.dot(ray.direction);
            if(vMag < 0) return false; // intersects *behind* ray!
            auto vMagSquared = vMag * vMag;
            auto cMag = cVec.dot(cVec);
            auto result = (radiusSquared - (cMag - vMagSquared));
            if(result < 0) return false;
            auto dist = vMag - sqrt(result);
            return r.swapDistance(dist,
                                  this);
        }
        virtual bool intersects(RayResult&) const override final;
        virtual void expandToFit(Box& b) const override final;
        virtual std::string print() override final;
    };
    
    std::ostream& operator<<(std::ostream&, const Sphere&);
}
