#pragma once
#include <array>

namespace NM {
    struct Vec4 {
        using BuffType = double[4];
        BuffType buff;

        constexpr inline Vec4(const Vec4& other) = default;
        constexpr inline Vec4(Vec4&& other) = default;
        constexpr inline Vec4(const double buff[4]) :
            buff{buff[0], buff[1], buff[2], buff[3]} {}

        constexpr inline Vec4(double x, double y, double z, double w = 1.0) :
            buff{x, y, z, w} {}

        inline double& x() {
            return buff[0];
        }

        inline constexpr double x() const {
            return buff[0];
        }

        inline double& y() {
            return buff[1];
        }

        inline constexpr double y() const {
            return buff[1];
        }

        inline double& z() {
            return buff[2];
        }

        inline constexpr double z() const {
            return buff[2];
        }

        inline double& w() {
            return buff[3];
        }

        inline constexpr double w() const {
            return buff[3];
        }

        inline double& operator[](int i) {
            return buff[i];
        }

        Vec4& operator=(const Vec4& other) = default;

        inline constexpr double operator[](int i) const {
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

        inline explicit operator BuffType&() {
            return buff;
        }

        inline Vec4& operator+=(const Vec4& other) {
            buff[0] += other[0];
            buff[1] += other[1];
            buff[2] += other[2];
            buff[3] += other[3];
            return *this;
        }

        inline Vec4& operator-=(const Vec4& other) {
            buff[0] -= other[0];
            buff[1] -= other[1];
            buff[2] -= other[2];
            buff[3] -= other[3];
            return *this;
        }

        template<typename T>
        inline typename std::enable_if<std::is_integral<T>::value, Vec4&>::type
        operator*=(T o) {
            buff[0] *= o;
            buff[1] *= o;
            buff[2] *= o;
            buff[3] *= o;
            return *this;
        }

        constexpr inline Vec4 operator+(const Vec4& other) const {
            return {
                buff[0] + other[0],
                buff[1] + other[1],
                buff[2] + other[2],
                buff[3] + other[3]
            };
        }

        constexpr inline Vec4 operator-(const Vec4& other) const {
            return {
                buff[0] - other[0],
                buff[1] - other[1],
                buff[2] - other[2],
                buff[3] - other[3]
            };
        }

        template<typename T>
        inline constexpr typename std::enable_if<std::is_arithmetic<T>::value, 
                                                Vec4>::type
        operator *(const T& obj) const {
            return {
                buff[0] * obj,
                buff[1] * obj,
                buff[2] * obj,
                buff[3] * obj
            };
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
            in * other[3]
        };
    }
}
