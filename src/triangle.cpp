#include <triangle.hpp>

namespace NM {
    
    RayIntersection Triangle::checkIntersection(const NM::Ray &ray) {
        Vec4 edgeA = (b - a);
        Vec4 edgeB = (c - a);
        Vec4 pvec = ray.direction.cross(edgeB);
        FloatType det = edgeA.dot(pvec);
        if(det > - Constants::epsilon && det < Constants::epsilon) {
            return {};
        }
        FloatType invDet = (1.0 / det);
        Vec4 toVert = ray.position - a;
        FloatType u = invDet * (toVert.dot(pvec));
        if(u < 0.0 || u > 1.0) {
            return {};
        }
        Vec4 q = toVert.cross(edgeA);
        FloatType v = invDet * ray.direction.dot(q);
        if(v < 0.0 || (u + v) > 1.0) {
            return {};
        }
        FloatType t = invDet * edgeB.dot(q);
        // This means that the triangle is on the same point as the ray
        // Due to how floats work, this can be close to zero but not zero.
        if(t > Constants::epsilon) {
            return {
                ray.position + (ray.direction * t),
                normal
            };
        }
        // There's an intersection but it's behind us
        return {};
        
    }
}
