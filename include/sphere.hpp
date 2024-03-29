#pragma once
#include <globals.hpp>
#include <drawable.hpp>
#include <vec4.hpp>
#include <material.hpp>


namespace NM {
    /**
     * @brief Represent a sphere in 3D space
     * @ingroup drawable
     */
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
        
        
  
        virtual Vec4 midpoint() const override final;
        virtual void swapInfo(RayResult&) const override final;
        
        Vec4 refractExit(const Ray& rayIn) const;
        
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
        virtual std::string print() const override final;
    };
    
    std::ostream& operator<<(std::ostream&, const Sphere&);
}
