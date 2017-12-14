#pragma once
#include <vec4.hpp>
#include <ostream>

namespace NM {
    /**
     * @brief Represents a point light.
     *
     * Point lights have a position and a color, and not much else.
     * This class models that.
     */
    struct Light {
        /**
         * @brief The position of the light in space.
         */
        Vec4 position;
        /**
         * @brief The color of the light.
         */
        Vec4 color;
        constexpr Light() = default;
        constexpr Light(const Vec4& pos, const Vec4& col) :
        position(pos), color(col) {};
    };
    
    std::ostream& operator<<(std::ostream&, const Light&);
}
