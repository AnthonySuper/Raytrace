#include <triangle.hpp>
#include <iostream>

namespace NM {
    
    RayIntersection Triangle::checkIntersection(const NM::Ray &ray) {
        Vec4 edgeA = (b - a);
        Vec4 edgeB = (c - a);
        // P = D x E2
        Vec4 pvec = ray.direction.cross(edgeB);
        // 1 / (P dot E) is the determinent of the matrix
        FloatType det = edgeA.dot(pvec);
        // If det is zero then we can't do this calculation, so 
        // our ray is oblique to the triangle, essentially
        if(det > - Constants::epsilon && det < Constants::epsilon) {
            return {};
        }
        // Find the inverse det to apply to each member of the vector
        FloatType invDet = (1.0 / det);
        // This is vector T in the original paper
        Vec4 toVert = ray.position - a;
        // If this u is less than zero or greater than one, bail early,
        // as we won't actually intersect the triangle
        // (this finds just one component of the eventual "solution vector"
        // so we can bail more quickly, basically)
        FloatType u = (toVert.dot(pvec)) * invDet;
        if(u < 0.0 || u > 1.0) {
            return {};
        }
        // Q in the original paper
        Vec4 q = toVert.cross(edgeA);
        // This is the v in baycentric coordinates
        FloatType v = invDet * ray.direction.dot(q);
        // One again, bail early if we're able
        if(v < 0.0 || (u + v) > 1.0) {
            return {};
        }
        // Finally find our T parameter 
        FloatType t = invDet * edgeB.dot(q);
        // If t = 0 we have the fun case where the ray origin is *exactly*
        // along the triangle. In this case, we consider no intersection to 
        // have taken place.
        if(t > Constants::epsilon) {
            return {
                ray.position + (ray.direction * t),
                normal,
                ray
            };
        }
        // There's an "intersection" but it's behind us or on our origin.
        // Either way, we don't care about it for purposes of raytracing.
        return {};
    }
    
    bool Triangle::operator==(const NM::Triangle &rhs) const {
        return a == rhs.a &&
        b == rhs.b &&
        c == rhs.c;
    }
    
    std::ostream& operator<<(std::ostream& os, const Triangle &t) {
        os << "[Triangle: " << t.a << "," << t.b << "," << t.c << "]";
        return os;
    }
    
    Triangle operator*(const Mat4& m, const Triangle& t) {
        return {
            m(t.a),
            m(t.b),
            m(t.c)
        };
    }
}
