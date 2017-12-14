#pragma once
#include <ostream>
#include <vec4.hpp>
#include <mat4.hpp>

namespace NM {
    /**
     * @brief models a ray, a geometric object with position and direction.
     * This is essentially a tuple of two vectors.
     */
    struct Ray {
        /**
         * The starting position of the ray.
         */
        Vec4 position;
        /**
         * @brief The direction of the ray.
         *
         * @warning By contract, this should always be unit-length.
         * This contract is not enforced at runtime, which may
         * change in the future.
         */
        Vec4 direction;
        /**
         * A simple, self-explanatory constructor:
         * Given a position and a direction, make a ray.
         */
        inline Ray(const Vec4& pos, const Vec4& dir) : 
            position(pos), direction(dir.toUnit()) {}
    };
    
    /**
     * @brief Output a ray to a stream in human-readable form.
     */
    std::ostream& operator<<(std::ostream&, const Ray&);
    /**
     * @brief Check if two rays are exactly equivalent.
     */
    bool operator==(const Ray&, const Ray&) noexcept;
    /**
     * @brief Transform a ray via a transformation matrix.
     */
    Ray operator*(const Mat4&, const Ray&);
}
