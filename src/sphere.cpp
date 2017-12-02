#include <sphere.hpp>
#include <iostream>
#include <material.hpp>

namespace NM {
    RayIntersection Sphere::checkIntersection(const Ray& ray) const {
        // get c parameter
        auto cVec = (position - ray.position);
        auto vMag = cVec.dot(ray.direction);
        if(vMag < 0) return {}; // intersects *behind* ray!
        auto vMagSquared = vMag * vMag;
        auto cMag = cVec.dot(cVec);
        auto result = (radiusSquared - (cMag - vMagSquared));
        if(result < 0) {
            return {}; // none found
        }
        auto q = ray.position + (vMag - sqrt(result))*ray.direction;
        return {q, (q - position).toUnit(), ray, material.get()};
    }
    
    size_t Sphere::complexity() const {
        return 1;
    }

    Vec4 Sphere::midpoint() const {
        return position;
    }

    bool Sphere::intersects(const Box& b) const {
        Vec4 clamped;
        for(int i = 0; i < 3; ++i) {
            clamped[i] = std::max(b.min[i],
                    std::min(position[i], b.max[i]));
        }
        auto toCenter = clamped - position;
        return toCenter.magnitude() < radius;
    }
    
    bool Sphere::intersects(RayResult& r) const {
        const auto& ray = r.originalRay;
        auto cVec = (position - ray.position);
        auto vMag = cVec.dot(ray.direction);
        if(vMag < 0) return false; // intersects *behind* ray!
        auto vMagSquared = vMag * vMag;
        auto cMag = cVec.magnitude();
        cMag *= cMag; // And also this
        auto result = (radiusSquared - (cMag - vMagSquared));
        if(result < 0) return false;
        auto dist = vMag - sqrt(result);
        if(! r.betterDistance(dist)) return false;
        auto q = ray.position + (vMag - sqrt(result))*ray.direction;
        auto norm = (q - position).toUnit();
        return r.swapDistance(dist,
                              norm,
                              material.get());
    }
    
    void Sphere::expandToFit(NM::Box &b) const {
        Vec4 directional{
            radius,
            radius,
            radius
        };
        b.expandToFit(position - directional);
        b.expandToFit(position + directional);
    }
    
    std::string Sphere::print() {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
    
    
    
    std::ostream& operator<<(std::ostream& os, const Sphere& s) {
        os << "{Sphere: pos " << s.position << ", rad: " << s.radius;
        /*
        if(s.material) {
            const Material& mat = *(s.material.get());
            os << ", Material: " << mat;
        }
         */
        os << "}";
        return os;
    }
}
