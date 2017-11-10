#pragma once
#include <globals.hpp>

namespace NM {
    class Color {
        /**
         * What type do we use for our internal buffer?
         */
        using BuffType = FloatType[3];
        BuffType buff;
    public:
        constexpr Color(FloatType r, FloatType g, FloatType b) :
        buff{r, g, b} {}
        
        constexpr Color(const Color&) = default;
        constexpr Color(Color&&) = default;
        constexpr Color& operator=(const Color&) = default;
        constexpr Color& operator=(Color&&) = default;
        
        constexpr void swap(Color& other) {
            Color tmp = other;
            other = *this;
            *this = tmp;
        }
        
        inline constexpr FloatType r() const noexcept {
            return buff[0];
        }
        
        inline constexpr FloatType& r()  noexcept {
            return buff[0];
        }
        
        inline constexpr FloatType g() const noexcept {
            return buff[1];
        }
        
        inline constexpr FloatType& g()noexcept {
            return buff[1];
        }
        
        inline constexpr FloatType b() const noexcept {
            return buff[2];
        }
        
        inline constexpr FloatType& b() noexcept {
            return buff[2];
        }
        
        inline FloatType& operator[](size_t i) noexcept {
            return buff[i];
        }
        
        inline FloatType operator[](size_t i) const noexcept {
            return buff[i];
        }
    };
    
    inline constexpr void swap(Color& a, Color&b) {
        return a.swap(b);
    }
}
