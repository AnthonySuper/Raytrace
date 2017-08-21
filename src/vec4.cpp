#include <vec4.hpp>

namespace NM {
    std::ostream& operator<<(std::ostream& os, const NM::Vec4& vec) {
        return os << "{" << \
            vec.x() << ", " << \
            vec.y() << ", " << \
            vec.z() << ", " << \
            vec.w() << "}";
    }
}
