#pragma once
#include <vec2.hpp>

namespace NM {
    struct Tri2 {
        Vec2 a, b, c;
        
        constexpr Tri2(const Vec2 &_a,
                       const Vec2 &_b,
                       const Vec2& _c) : a{_a}, b{_b}, c{_c}
        {}
        
        constexpr Tri2() = default;
        constexpr Tri2(const Tri2&) = default;
        
    };
}
