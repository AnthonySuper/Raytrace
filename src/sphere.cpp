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
        auto cMag = cVec.magnitude();
        cMag *= cMag; // And also this
        auto result = (radiusSquared - (cMag - vMagSquared));
        if(result < 0) {
            return {}; // none found
        }
        auto q = ray.position + (vMag - sqrt(result))*ray.direction;
        return {q, (q - position).toUnit(), ray, material.get()};
    }
    
    std::ostream& operator<<(std::ostream& os, const Sphere& s) {
        os << "{Sphere: pos " << s.position << ", rad: " << s.radius;
        if(s.material) {
            const Material& mat = *(s.material.get());
            os << ", Material: " << mat;
        }
        os << "}";
        return os;
    }
}
