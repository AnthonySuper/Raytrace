#pragma once
#include <mat4.hpp>
#include <cmath>

namespace NM {
    /**
     * Namespace containing free functions that create various transformations.
     */
    namespace Transform {
        /**
         * Create a transformation which translates by a constant factor.
         * @param by the vector to use for translation
         */
        inline constexpr Mat4 translate(const Vec4& by) noexcept {
            const auto v = by.normalized();
            return {1, 0, 0, v[0],
                0, 1, 0, v[1],
                0, 0, 1, v[2],
                0, 0, 0, 1};
        }
        
        /**
         * @brief create a transformation that scales by a non-constant factor.
         */
        inline constexpr Mat4 scale(const Vec4& by) {
            const auto v = by.normalized();
            return {
                v[0], 0, 0, 0,
                0, v[1], 0, 0,
                0, 0, v[2], 0,
                0, 0, 0, 1
            };
        }
        
        /**
         * @brief create a transformation that sclaes by a constant.
         */
        inline constexpr Mat4 scale(const FloatType by) {
            return {
                by, 0, 0, 0,
                0, by, 0, 0,
                0, 0, by, 0,
                0, 0, 0, 1
            };
        }
        
        /**
         * @brief create a transformation which rotates around an axis
         */
        Mat4 axisAngle(Vec4 axis, FloatType angle);
        
        /**
         * @brief Rotate around Z by some degres
         * @param angle the angle to rotate by in degrees
         */
        Mat4 rotateZ(FloatType angle) noexcept;
    }
}
