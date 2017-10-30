#pragma once
#include <vec4.hpp>
#include <globals.hpp>
#include <string>
#include <sstream>
#include <cmath>

namespace NM {
    /**
     * @brief A 4x4 matrix, which can also be considered a transformation.
     */
    struct Mat4 {
        constexpr static float InverseZeroBound = 0.000001;
        using RowType = FloatType[4];
        FloatType data[4][4];

        /**
         * Default constructor returns a zero matrix
         */
        inline constexpr Mat4() : data{{0}} {}
        /**
         * Copy constructor is defauled
         */
        inline Mat4(const Mat4& other) = default;
        /**
         * Move constructor is default
         */
        inline Mat4(Mat4&& other) = default;

        inline constexpr Mat4(const RowType &row1, const RowType &row2,
                const RowType &row3, const RowType &row4) :
        data{{row1[0], row1[1], row1[2], row1[3]},
            {row2[0], row2[1], row2[2], row2[3]},
            {row3[0], row3[1], row3[2], row3[3]},
            {row4[0], row4[1], row4[2], row4[3]}} {}
        
        inline constexpr Mat4(const Vec4 r1,
                              const Vec4 r2,
                              const Vec4 r3,
                              const Vec4 r4) :
        data{
            {r1[0], r1[1], r1[2], r1[3]},
            {r2[0], r2[1], r2[2], r2[3]},
            {r3[0], r3[1], r3[2], r3[3]},
            {r4[0], r4[1], r4[2], r4[3]}
        } {}
        
        // could probably be done with templates, oh well...
        inline constexpr Mat4(double a0, double a1, double a2, double a3,
                double b0, double b1, double b2, double b3,
                double c0, double c1, double c2, double c3,
                double d0, double d1, double d2, double d3) :
        data{{a0, a1, a2, a3},
            {b0, b1, b2, b3},
            {c0, c1, c2, c3},
            {d0, d1, d2, d3}} {}

        inline constexpr const RowType& operator[](int i) const {
            return data[i];
        }

        inline constexpr RowType& operator[](int i) {
            return data[i];
        }

        constexpr const Vec4 rowAt(int i) const {
            return data[i];
        }

        constexpr Vec4 rowAt(int i) noexcept {
            return Vec4(data[i]);
        }

        constexpr double at(int i, int j) const noexcept {
            return data[i][j];
        }

        Mat4& operator=(const Mat4& other) = default;

        inline constexpr Mat4 operator+(const Mat4& other) const noexcept {
            Mat4 tmp;
            for(int i = 0; i < 4; ++i) {
                for(int k = 0; k < 4; ++k) {
                    tmp[i][k] = data[i][k] + other[i][k];
                }
            }
            return tmp;
        }

        inline constexpr Mat4& operator+=(const Mat4& other) noexcept {
            for(int i = 0; i < 4; ++i) {
                for(int k = 0; k < 4; ++k) {
                    data[i][k] += other[i][k];
                }
            }
            return *this;
        }

        inline constexpr Mat4 operator*(const Mat4& other) const noexcept {
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

        inline constexpr Mat4& operator*=(const Mat4& other) noexcept {
            *this = (*this * other);
            return *this;
        }

        inline constexpr Vec4 operator*(const Vec4& other) const noexcept {
            Vec4 d{0, 0, 0};
            for(int i = 0; i < 4; ++i) {
                FloatType tmp = 0;
                for(int k = 0; k < 4; ++k) {
                    tmp += data[i][k] * other[k];
                }
                d[i] = tmp;
            }
            return d;
        }
        
        inline Vec4 normalTransform(const Vec4& other) const noexcept {
            Vec4 d{};
            Mat4 omat = inverse().transpose();
            for(int i = 0; i < 3; ++i) {
                FloatType tmp = 0;
                for(int k = 0; k < 3; ++k) {
                    if(i < 3 && k == 3) continue;
                    tmp += omat.data[i][k] * other[k];
                }
                d[i] = tmp;
            }
            // d[3] = 1;
            return d;
        }

        inline constexpr bool operator==(const Mat4& other) const noexcept {
            for(int i = 0; i < 4; ++i) {
                for(int k = 0; k < 4; ++k) {
                    if(data[i][k] != other[i][k]) {
                        return false;
                    }
                }
            }
            return true;
        }

        inline constexpr static Mat4 id() noexcept {
            return {1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1};
        }
        
        /**
         * @brief Apply a transformation to a vector.
         */
        inline constexpr Vec4 operator()(const Vec4& in) const noexcept {
            return (*this) * in;
        }
        
        inline constexpr Mat4 operator()(const Mat4& in) const {
            return (*this) * in;
        }
        
        template<typename T>
        inline T apply(const T& in) const {
            return (*this) * in;
        }
        
        inline constexpr Mat4 transpose() const noexcept {
            Mat4 toRet;
            for(int i = 0; i < 4; ++i) {
                for(int k = 0; k < 4; ++k) {
                    toRet[k][i] = (*this)[i][k];
                }
            }
            return toRet;
        }
        
        Mat4 truncedZeros() const;
        
        /**
         * @brief An exception to be thrown when a Matrix cannot be inverted.
         */
        class NonInvertableError: std::runtime_error {
        public:
            NonInvertableError() :
                runtime_error("Matrix is not inverable")
            {}
        };
        
        /**
         * @brief Get the inverse of a matrix.
         * 
         * This code is modified from the MESA library.
         * This library is licensed under the MIT license.
         *
         * @throws NonInvertableError
         */
        Mat4 inverse() const;
        
        std::string toString() const;
        
    private:
        static FloatType truncToZero(FloatType in);
    };

    std::ostream& operator<<(std::ostream& os, Mat4 const & mat);
}
