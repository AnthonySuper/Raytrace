#pragma once
#include <array>
#include <globals.hpp>
#include <ostream>
#include <istream>
#include <stdexcept>
#include <errors.hpp>

namespace NM {
    
    /**
     * Represent a vector in homogenous coordinates
     */
    struct Vec4 {
        /**
         * Get the height of this vector, considered as a matrix
         */
        constexpr static int height = 4;
        /**
         * Get the width of this vector, considered as a matrix
         */
        constexpr static int width = 1;
        /**
         * What type do we use for the buffer?
         */
        using BuffType = FloatType[4];
        /**
         * The actual buffer for the member variables
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
         * Create a vector from an array of floats
         */
        constexpr inline Vec4(const FloatType buff[4]) :
            buff{buff[0], buff[1], buff[2], buff[3]} {}

        /**
         * Create a vector with given values.
         * W defaults to zero.
         */
        constexpr inline Vec4(FloatType x, 
                              FloatType y, 
                              FloatType z, 
                              FloatType w = 1.0) :
            buff{x, y, z, w} {}

        inline FloatType& x() {
            return buff[0];
        }

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

		inline constexpr FloatType r() const {
			return buff[0];
		}

		inline constexpr FloatType g() const {
			return buff[1];
		}

		inline constexpr FloatType b() const {
			return buff[2];
		}

		inline constexpr FloatType a() const {
			return buff[3];
		}

        inline constexpr FloatType& operator[](int i) {
            return buff[i];
        }

        Vec4& operator=(const Vec4& other) = default;

        inline constexpr Vec4 normalized() const {
            if(w() == 1.0) return *this;
            if(w() == 0)
                throw std::overflow_error("Normalize with w=0");
            auto w = this->w();
            return {x() / w, y() / w, z() / w, 1.0};
        }
        
        inline FloatType magnitude() const {
            const auto square = [](FloatType in) { return in * in; };
            const auto norm = normalized();
            const auto sum = square(norm.x()) +
                square(norm.y()) +
                square(norm.z());
            return std::sqrt(sum);
        }
        
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

        inline operator BuffType&() {
            return buff;
        }

        inline Vec4& operator+=(const Vec4& other) {
            buff[0] += other[0];
            buff[1] += other[1];
            buff[2] += other[2];
            return *this;
        }

        inline Vec4& operator-=(const Vec4& other) {
            buff[0] -= other[0];
            buff[1] -= other[1];
            buff[2] -= other[2];
            return *this;
        }
        
        inline constexpr Vec4 operator-() const {
            return {
                - buff[0],
                - buff[1],
                - buff[2]
            };
        }
        
        inline Vec4 cross(const Vec4& other) const  {
            auto u = *this;
            auto v = other;
            return {
                u[1]*v[2] - u[2]*v[1],
                u[2]*v[0] - u[0]*v[2],
                u[0]*v[1] - u[1]*v[0],
            };
        }

        template<typename T>
        inline typename std::enable_if<std::is_integral<T>::value, Vec4&>::type
        operator*=(T o) {
            buff[0] *= o;
            buff[1] *= o;
            buff[2] *= o;
            return *this;
        }

        constexpr inline Vec4 operator+(const Vec4& other) const {
            return {
                buff[0] + other[0],
                buff[1] + other[1],
                buff[2] + other[2],
            };
        }

        constexpr inline Vec4 operator-(const Vec4& other) const {
            return {
                buff[0] - other[0],
                buff[1] - other[1],
                buff[2] - other[2]
            };
        }
        
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
        
        inline constexpr FloatType dot(const Vec4& other) const {
            return buff[0] * other[0] +
            buff[1] * other[1] +
            buff[2] * other[2];
        }

    };

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


    std::ostream& operator<<(std::ostream& os, NM::Vec4 const & vec);
    
    std::istream& operator>>(std::istream& is, Vec4 & vec);
    
}

