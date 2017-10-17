#include <triangle.hpp>
#include <iostream>

namespace NM {
    
   
    
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
