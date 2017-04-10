#pragma once
#include <vec4.hpp>


namespace NM {
    struct Mat4 {
        using ItemType = double;
        using RowType = ItemType[4];
        ItemType data[4][4];

        inline constexpr Mat4() : data{0} {}

        inline Mat4(const Mat4& other) = default;
        inline Mat4(Mat4&& other) = default;

        inline constexpr Mat4(const RowType &row1, const RowType &row2,
                const RowType &row3, const RowType &row4) : 
            data{row1[0], row1[1], row1[2], row1[3],
                row2[0], row2[1], row2[2], row2[3],
                row3[0], row3[1], row3[2], row3[3],
                row4[0], row4[1], row4[2], row4[3]} {}

        // could probably be done with templates, oh well...
        inline constexpr Mat4(double a0, double a1, double a2, double a3,
                double b0, double b1, double b2, double b3,
                double c0, double c1, double c2, double c3,
                double d0, double d1, double d2, double d3) :
            data{a0, a1, a2, a3,
                b0, b1, b2, b3,
                c0, c1, c2, c3,
                d0, d1, d2, d3} {}

        inline constexpr const RowType& operator[](int i) const {
            return data[i];
        }

        inline constexpr RowType& operator[](int i) {
            return data[i];
        }

        constexpr const Vec4 rowAt(int i) const {
            return data[i];
        }

        constexpr Vec4 rowAt(int i) {
            return Vec4(data[i]);
        }

        constexpr double at(int i, int j) const {
            return data[i][j];
        }

        Mat4& operator=(const Mat4& other) = default;

        inline constexpr Mat4 operator+(const Mat4& other) const {
            Mat4 tmp;
            for(int i = 0; i < 4; ++i) {
                for(int k = 0; k < 4; ++k) {
                    tmp[i][k] = data[i][k] + other[i][k];
                }
            }
            return tmp;
        }

        inline constexpr Mat4& operator+=(const Mat4& other) {
            for(int i = 0; i < 4; ++i) {
                for(int k = 0; k < 4; ++k) {
                    data[i][k] += other[i][k];
                }
            }
            return *this;
        }

        inline constexpr Mat4 operator*(const Mat4& other) const {
            Mat4 buff;
            for(int i = 0; i < 4; ++i) {
                for(int j = 0; j < 4; ++j) {
                    double tmp = 0;
                    for(int k = 0; k < 4; ++k) {
                        tmp += data[i][k] * other[k][j];
                    }
                    buff[i][j] = tmp;
                }
            }
            return buff;
        }

        inline constexpr Mat4& operator*=(const Mat4& other) {
            *this = (*this * other);
            return *this;
        }

        inline constexpr Vec4 operator*(const Vec4& other) const {
            Vec4 d{0, 0, 0};
            for(int i = 0; i < 4; ++i) {
                double tmp = 0;
                for(int k = 0; k < 4; ++k) {
                    tmp += data[i][k] * other[i];
                }
                d[i] = tmp;
            }
            return d;
        }

        inline constexpr bool operator==(const Mat4& other) const {
            for(int i = 0; i < 4; ++i) {
                for(int k = 0; k < 4; ++k) {
                    if(data[i][k] != other[i][k]) {
                        return false;
                    }
                }
            }
            return true;
        }

        inline constexpr static Mat4 id() {
            return {1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1};
        }
    };
}
