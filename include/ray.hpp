#pragma once
#include <ostream>
#include <vec4.hpp>

namespace NM {
    struct Ray {
        Vec4 position;
        Vec4 direction;
        inline Ray(const Vec4& pos, const Vec4& dir) : 
            position(pos), direction(dir.toUnit()) {}
    };
    
    std::ostream& operator<<(std::ostream&, const Ray&);
    bool operator==(const Ray&, const Ray&) noexcept;
}
