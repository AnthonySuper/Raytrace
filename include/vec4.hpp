#pragma once
#include <array>
#include <globals.hpp>
#include <ostream>
#include <istream>
#include <stdexcept>
#include <errors.hpp>

namespace NM {

    /**
     * @brief Represent a vector in 3D homogenous coordinates.
     *
     * This type is one of the core types of the Raytracer.
     * It is used throughout.
     * Currently, it is not optimized to use SIMD directly, however,
     * the compiler does emit some SIMD instructions when compiling.
     */
    struct Vec4 {
        /**
         * @brief The height of this type, considered as a matrix
         */
        constexpr static int height = 4;
        /**
         * @brief The width of this type, considered as a matrix
         */
        constexpr static int width = 1;

        /**
         * @brief a description of the axises in 3D homogenous space
         * represented by this vector.
         *
         * This convenience type is more readable than passing indexes.
         */
        enum class Axis {
            x = 0,
            y = 1,
            z = 2,
            w = 3
        };
        /**
         * @brief The internal buffer type for our member data.
         *
         * We store this directly as an array of four values as opposed to
         * separate X, Y, Z, and W values because it is probably easier to 
         * optimize. It also means that we can convert to SIMD intrinsics
         * much more easily.
         */
        using BuffType = FloatType[4];
        /**
         * @brief The actual data buffer
         *
         * See our typedef of BuffType for why we store our values like
         * this.
         */
        BuffType buff;

        constexpr inline Vec4() : buff{0, 0, 0, 1} {}

        /**
         * Default copy constructor
         */
        constexpr inline Vec4(const Vec4& other) = default;
        /**
         * Default move constructor
         */
        constexpr inline Vec4(Vec4&& other) = default;
        /**
         * @brief Create a vector from an array of floats
         */
        constexpr inline Vec4(const FloatType buff[4]) :
            buff{buff[0], buff[1], buff[2], buff[3]} {}

        /**
         * @brief Create a vector with given values.
         * W defaults to zero.
         *
         */
        constexpr inline Vec4(FloatType x, 
                FloatType y, 
                FloatType z, 
                FloatType w = 1.0) :
            buff{x, y, z, w} {}

        /**
         * @brief Return a mutable reference to this vector's X value.
         *
         * Used pretty often.
         */
        inline FloatType& x() {
            return buff[0];
        }

        /**
         * @brief Return the value of this vector's X value.
         */
        inline constexpr FloatType x() const {
            return buff[0];
        }

        inline FloatType& y() {
            return buff[1];
        }

        inline constexpr FloatType y() const {
            return buff[1];
        }

        inline FloatType& z() {
            return buff[2];
        }

        inline constexpr FloatType z() const {
            return buff[2];
        }

        inline FloatType& w() {
            return buff[3];
        }

        inline constexpr FloatType w() const {
            return buff[3];
        }

        /**
         * @brief When considered as a color, return the red component.
         *
         * Equivalent to #x
         */
        inline constexpr FloatType r() const {
            return buff[0];
        }

        /**
         * @brief when considered as a color, return the green component.
         *
         * Equivalent to #y
         */
        inline constexpr FloatType g() const {
            return buff[1];
        }

        /**
         * @brief when considered as a color, return the blue component.
         * 
         * Equivalent to #z()
         */
        inline constexpr FloatType b() const {
            return buff[2];
        }

        /**
         * @brief when considered as a color, return the alpha component.
         *
         * Eqvuialent to #w()
         */
        inline constexpr FloatType a() const {
            return buff[3];
        }

        /**
         * @brief get a component in the vector
         *
         * @warning does not do bounds checking!
         */
        inline constexpr FloatType& operator[](int i) {
            return buff[i];
        }

        /**
         * @brief get a component in the vector
         *
         * @warning does not do bounds checking!
         */
        inline constexpr FloatType& operator[](Axis ax) {
            return buff[static_cast<int>(ax)];
        }

        Vec4& operator=(const Vec4& other) = default;

        /**
         * @brief return a normalized vector
         *
         * In this case, "normalized" means "with w=1."
         * If you want it unit-length, use #toUnit
         */
        inline constexpr Vec4 normalized() const {
            if(w() == 1.0) return *this;
            if(w() == 0)
                throw std::overflow_error("Normalize with w=0");
            auto w = this->w();
            return {x() / w, y() / w, z() / w, 1.0};
        }

        /**
         * Get the magnitude of this vector.
         * Uses the dot product!
         */
        inline FloatType magnitude() const {
            return std::sqrt(dot(*this));
        }

        /**
         * @brief This vector, but unit length.
         *
         * @return a new vector equivalent to this one but unit length
         * @throws std::overflow_error if this vector is {0, 0, 0}
         */
        inline Vec4 toUnit() const {
            auto ref = normalized();
            auto mag = ref.magnitude();
            if(mag == 0) throw std::overflow_error("Zero Magnitude");
            return {
                x() / mag,
                    y() / mag,
                    z() / mag
            };
        }

        inline constexpr FloatType operator[](Axis a) const {
            return buff[static_cast<int>(a)];
        }

        inline constexpr FloatType operator[](int i) const {
            return buff[i];
        }

        inline constexpr bool operator==(const Vec4& other) const {
            return buff[0] == other.buff[0] &&
                buff[1] == other.buff[1] &&
                buff[2] == other.buff[2] &&
                buff[3] == other.buff[3];
        }

        inline constexpr bool operator!=(const Vec4& other) const {
            return ! (*this == other);
        }

        /**
         * @brief Convert this vector to its raw buffer type.
         */
        inline operator BuffType&() {
            return buff;
        }

        /**
         * @brief add vectors together in-place.
         *
         * This does not consider the w component.
         */
        inline Vec4& operator+=(const Vec4& other) {

            for(int i = 0; i < 3; ++i) {
                buff[i] += other[i];
            }
            return *this;
        }

        /**
         * @brief in-place subtraction.
         *
         * Slightly faster than the non-in-place verson, maybe?
         */
        inline Vec4& operator-=(const Vec4& other) {
            buff[0] -= other[0];
            buff[1] -= other[1];
            buff[2] -= other[2];
            return *this;
        }


        /**
         * @brief negate this vector
         *
         * This returns a vector pointing in the opposite direction with
         * the same magnitude.
         */
        inline constexpr Vec4 operator-() const {
            return {
                - buff[0],
                - buff[1],
                - buff[2]
            };
        }

        /**
         * @brief get the cross product
         *
         * This returns the 3D cross product, not considering w.
         */
        inline Vec4 cross(const Vec4& other) const  {
            auto u = *this;
            auto v = other;
            return {
                u[1]*v[2] - u[2]*v[1],
                u[2]*v[0] - u[0]*v[2],
                u[0]*v[1] - u[1]*v[0],
            };
        }

        /**
         * @brief perform the scalar product
         *
         * This template is defined for all integral values.
         */
        template<typename T>
            inline typename std::enable_if<std::is_integral<T>::value, Vec4&>::type
            operator*=(T o) {
                buff[0] *= o;
                buff[1] *= o;
                buff[2] *= o;
                return *this;
            }





        /**
         * @brief returns a "fuzzy equality" value (equal within epsilon)
         *
         * Mostly used for testing.
         */
        constexpr inline bool fuzzyEquals(const Vec4& other) const {
            return fuzzyCompare(buff[0], other[0]) &&
                fuzzyCompare(buff[1], other[1]) &&
                fuzzyCompare(buff[2], other[2]);
        }

        template<typename T>
            inline constexpr typename std::enable_if<std::is_arithmetic<T>::value, 
                   Vec4>::type
                       operator *(const T& obj) const {
                           return {
                               buff[0] * obj,
                               buff[1] * obj,
                               buff[2] * obj,
                           };
                       }

        /**
         * @brief scalar product
         *
         * This is a specialization for doubles to make deduction easier.
         */
        Vec4 operator *(const double num) const {
            return {
                buff[0] * num,
                buff[1] * num,
                buff[2] * num
            };
        }

        /**
         * @brief performs the dot product
         *
         * This is considered in 3D, and does not take #w into account.
         */
        inline constexpr FloatType dot(const Vec4& other) const {
            FloatType accum = 0;
            for(int i = 0; i < 3; ++i) {
                accum += buff[i] * other.buff[i];
            }
            return accum;
        }

        /**
         * @brief performs the pairwise product
         *
         * Almost always done considered as a color.
         */
        inline constexpr Vec4 pairwiseProduct(const Vec4& other) const {
            return {
                buff[0] * other[0],
                buff[1] * other[1],
                buff[2] * other[2]
            };
        }

        /**
         * @brief convert all values to positive
         *
         * Only really useful when considered as a color.
         */
        inline void positiveize() {
            for(int i = 0; i < 3; ++i) {
                buff[i] = std::fabs(buff[i]);
            }
        }

    };

    /**
     * @brief out-of-line definition of multiplicaton.
     */
    template<typename T>
        inline constexpr typename std::enable_if<std::is_arithmetic<T>::value,
               Vec4>::type
                   operator *(T in, const Vec4 other) {
                       return {
                           in * other[0],
                           in * other[1],
                           in * other[2],
                       };
                   }


    /**
     * @brief component-wise minimum.
     *
     * This is useful for AABBs. See Box.
     * @todo make this variadic 
     */
    inline constexpr Vec4 pseudoMin(const Vec4 &a, const Vec4& b) {
        return {
            std::min(a[0], b[0]),
            std::min(a[1], b[1]),
            std::min(a[2], b[2])
        };
    }

    /**
     * @brief component-wise maximum.
     *
     * This is useful for AABBS. See Box.
     * @todo make this variadic
     */
    inline constexpr Vec4 pseudoMax(const Vec4 &a, const Vec4& b) {
        return {
            std::max(a[0], b[0]),
                std::max(a[1], b[1]),
                std::max(a[2], b[2])
        };
    }

    /**
     * @brief 3D Vector addition
     *
     * This is geometrically only really useful in point-vector
     * addition, but the algorithm is the same either way.
     */
    constexpr inline Vec4 operator+(const Vec4& lhs,
            const Vec4& rhs) {
        return {
            lhs[0] + rhs[0],
            lhs[1] + rhs[1],
            lhs[2] + rhs[2],
        };
    }


    /**
     * @brief 3D vector subtraction.
     *
     * This ignores the #w paramter for convenience.
     * The return value can be considered either a true vector or a point.
     * If you subtract two vectors, the result is garbage geometrically.
     * If you subtract a point from a point, you get a vector.
     * If you subtract a vector from a point, you get another point.
     *
     * Thankfully, the formula to do this is the same no matter what
     * the geometric interpretation is.
     */
    constexpr inline Vec4 operator-(const Vec4& lhs, const Vec4& rhs) {
        return {
            lhs[0] - rhs[0],
            lhs[1] - rhs[1],
            lhs[2] - rhs[2]
        };
    }

    /**
     * Output a vector in human-readable format.
     */
    std::ostream& operator<<(std::ostream& os, NM::Vec4 const & vec);

    std::istream& operator>>(std::istream& is, Vec4 & vec);

}

