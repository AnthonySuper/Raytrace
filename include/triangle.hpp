#pragma once
#include <vec4.hpp>
#include <mat4.hpp>
#include <ray_intersection.hpp>
#include <ostream>

namespace NM {
    struct Triangle {
        Vec4 a, b, c, normal;
        inline Triangle(const Vec4& _a, const Vec4& _b, const Vec4& _c) :
        a(_a), b(_b), c(_c), normal((a - b).cross(c - b))
        {}
        
        inline Triangle& apply(const Mat4 &mat) {
            a = mat(a);
            b = mat(b);
            c = mat(c);
            // TODO: See if we can just apply it to the normal as well
            mat.normalTransform(normal);
            return *this;
        }
        
        inline void calcNormal() {
            normal = (a - b).cross(c - b);
        }
        
        bool operator==(const Triangle& rhs) const;
        /**
         @brief Check for the intersection between this triangle and a ray
         
         This uses the
         <a href="http://webserver2.tecgraf.puc-rio.br/~mgattass/cg/trbRR/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf">
         "Fast Minimum Storage Ray/Triangle Intersection"
         </a>
         algorithm.
         This algorithm essentially uses some geometry to move the triangle
         to the origin of the ray, then algebriac trickery to do
         Cramer's rule must faster than normally possible.
         
         coordOut returns where the ray hit the triangle in baycentric coordinates.
         coordOut[x] refers to the distance alone (ba).
         coordOut[y] refers to the distance along (ca)
         coordOut[z] refers to the distance along (cb)
         */
        inline RayIntersection checkIntersection(const Ray& ray) const {
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
        
        Vec4 toBarycentric(const Vec4& point) const;
    };
    
    Triangle operator*(const Mat4&, const Triangle&);
    
    
    
    std::ostream& operator<<(std::ostream&, const Triangle&);
}
