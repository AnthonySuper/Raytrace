#include <transforms.hpp>
#include <iostream>

namespace NM {
    namespace Transform {
        Mat4 rotateZ(FloatType angle) noexcept {
            return {
                std::cos(angle), -std::sin(angle), 0, 0,
                std::sin(angle), std::cos(angle), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }
        
        Mat4 axisAngle(Vec4 axis, FloatType angle) {
            Vec4 w = axis.toUnit();
            Vec4 tmp = axis;
            if(tmp[0] != 1.0) tmp[0] = 1.0;
            else {
                // cances of somebody pointing in this direction are
                // probably really small, right?
                tmp[0] = 0.000123123;
                tmp[1] = 0.12312;
                tmp[2] = 123;
            }
            tmp = tmp.toUnit();
            Vec4 u = w.cross(tmp).toUnit();
            Vec4 v = - u.cross(w).toUnit();
            Mat4 rw{
                u[0], u[1], u[2], 0,
                v[0], v[1], v[2], 0,
                w[0], w[1], w[2], 0,
                0, 0, 0, 1
            };
            Mat4 rot = rotateZ(angle);
            return rw.transpose() * rot * rw;
        }
    }
}
