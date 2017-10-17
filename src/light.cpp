#include <light.hpp>

namespace NM {
    std::ostream& operator<<(std::ostream& os, const Light& l) {
        os << "{Light: Pos " << l.position << ", color " << l.color;
        return os << "}";
    }
}
