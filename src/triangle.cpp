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
    
    Vec4 Triangle::toBarycentric(const NM::Vec4 &point) const {
        Vec4 v0 = (b - a), v1 = (c - a), v2 = (point - a);
        auto d00 = v0.dot(v0);
        auto d01 = v0.dot(v1);
        auto d11 = v1.dot(v1);
        auto d20 = v2.dot(v0);
        auto d21 = v2.dot(v1);
        auto denom = d00 * d11 - d01 * d01;
        auto mult = 1.0 / denom;
        auto v = (d11 * d20 - d01 * d21) * mult;
        auto w = (d00 * d21 - d01 * d20) * mult;
        auto u = 1.0 - v - w;
        return {v, w, u};
    }
}
