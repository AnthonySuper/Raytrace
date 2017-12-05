#include <vec2.hpp>


namespace NM {
    std::ostream& operator<<(std::ostream& os, const Vec2 & v) {
        return os << "{" << v.u() << "," << v.v() << "}";
    }
}
