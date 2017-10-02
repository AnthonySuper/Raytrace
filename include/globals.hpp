#pragma once

#include <cmath>

namespace NM {
	using FloatType = double;
    
    inline FloatType inverseSqrt(FloatType in) {
        return 1.0 / std::sqrt(in);
    }
    
    inline FloatType degToRad(FloatType deg) {
        return deg * (M_PI) / 180.0;
    }
    
    inline constexpr FloatType constexprAbs(FloatType f) {
        if(f < 0) return (0 - f);
        return f;
    }
    
    inline constexpr bool fuzzyCompare(FloatType a, FloatType b, FloatType epsilon = 0.0001) {
        return constexprAbs(a - b) < epsilon;
    }
}
