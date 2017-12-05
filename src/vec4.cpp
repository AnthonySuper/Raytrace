#include <vec4.hpp>

namespace NM {
    std::ostream& operator<<(std::ostream& os, const NM::Vec4& vec) {
        return os << "{" << \
            vec.x() << ", " << \
            vec.y() << ", " << \
            vec.z() << ", " << \
            vec.w() << "}";
    }
    
    std::istream& operator>>(std::istream& is, Vec4& vec) {
        FloatType x, y, z, w;
        is >> x;
        is >> y;
        is >> z;
        if(! is) {
            vec = {x, y, (1 - x - y)};
            return is;
        }
        is >> w;
        if( ! is) {
            vec = {x, y, z};
        }
        else {
            vec = {x, y, z, w};
        }
        return is;
    }
}
