#pragma once
#include <globals.hpp>

namespace NM {
    struct Vec2 {
        using BuffType = FloatType[2];
        BuffType buff;
        
        constexpr Vec2(FloatType u, FloatType v) : buff{u, v} {}
        
        constexpr Vec2() : buff{0, 0} {}
        
        constexpr Vec2(BuffType b) :
        buff{b[0], b[1]} {}
        
        constexpr operator BuffType& () {
            return buff;
        }
        
        constexpr const operator const BuffType&() const {
            return buff;
        }
        
        constexpr FloatType& u() {
            return buff[0];
        }
        
        constexpr FloatType& v() {
            return buff[1];
        }
        
        constexpr const FloatType& u() const {
            return buff[0];
        }
        
        constexpr const FloatType& v() const {
            return buff[1];
        }
        
        constexpr const FloatType operator[](size_t i) const {
            return buff[i];
        }
        
        constexpr FloatType& operator[](size_t i) {
            return buff[i];
        }
        
        constexpr Vec2 operator*(FloatType t) {
            return {t * buff[0], t * buff[1] };
        }
        
        constexpr Vec2 operator+(const Vec2 &o) {
            return {
                buff[0] + o.buff[0],
                buff[1] + o.buff[1]
            };
        }
        
        constexpr Vec2 operator-() {
            return {-buff[0], -buff[1]};
        }
    };
    
    std::ostream& operator<<(std::ostream&, const Vec2&);
}
