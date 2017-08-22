#include <mat4.hpp>

namespace NM {
    namespace Transform {
        constexpr Mat4 translate(const Vec4& by) {
            auto v = by.normalized();
            return {1, 0, 0, v.x(),
                0, 1, 0, v.y(),
                0, 0, 1, v.z(),
                0, 0, 0, 1};
        }
    }
}
