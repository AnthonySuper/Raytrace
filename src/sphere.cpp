#include <sphere.hpp>
#include <iostream>

namespace NM {
    RayIntersection Sphere::checkIntersection(const Ray& ray) const {
        // get c parameter
        auto cVec = (position - ray.position);
        auto vMag = cVec.dot(ray.direction);
        auto vMagSquared = vMag * vMag;
        auto cMag = cVec.magnitude();
        cMag *= cMag; // And also this
        auto result = (radiusSquared - (cMag - vMagSquared));
        if(result < 0) {
            return {}; // none found
        }
        auto q = ray.position + (vMag - sqrt(result))*ray.direction;
        return {q, (position - q).normalized(), ray};
    }
}
