#pragma once
#include <vec4.hpp>
#include <mat4.hpp>
#include <ray_intersection.hpp>

namespace NM {
    struct Triangle {
        Vec4 a, b, c, normal;
        inline constexpr Triangle(const Vec4& _a, const Vec4& _b, const Vec4& _c) :
        a(_a), b(_b), c(_c), normal((a - b).cross(c - b))
        {}
        inline Triangle& apply(const Mat4 &mat) {
            a = mat(a);
            b = mat(b);
            c = mat(c);
            // TODO: See if we can just apply it to the normal as well
            calcNormal();
            return *this;
        }
        
        inline void calcNormal() {
            normal = (a - b).cross(c - b);
        }
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
         */
        RayIntersection checkIntersection(const Ray& ray);
    };
}
