#include <mat4.hpp>
#include <cmath>

namespace NM {
    namespace Transform {
        constexpr Mat4 translate(const Vec4& by) {
            const auto v = by.normalized();
            return {1, 0, 0, v[0],
                0, 1, 0, v[1],
                0, 0, 1, v[2],
                0, 0, 0, 1};
        }
        
        constexpr Mat4 scale(const Vec4& by) {
            const auto v = by.normalized();
            return {
                v[0], 0, 0, 0,
                0, v[1], 0, 0,
                0, 0, v[2], 0,
                0, 0, 0, 1
            };
        }
        
        constexpr Mat4 scale(const double by) {
            return {
                by, 0, 0, 0,
                0, by, 0, 0,
                0, 0, by, 0,
                0, 0, 0, 1
            };
        }
    }
}
