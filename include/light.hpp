#pragma once
#include <vec4.hpp>
#include <ostream>

namespace NM {
    struct Light {
        Vec4 position;
        Vec4 color;
        constexpr Light() = default;
        constexpr Light(const Vec4& pos, const Vec4& col) :
        position(pos), color(col) {};
    };
    
    std::ostream& operator<<(std::ostream&, const Light&);
}
