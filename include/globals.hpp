#pragma once

#include <cmath>

namespace NM {
	using FloatType = double;
    
    struct Constants {
        /**
         Float epsilon for comparison
         */
        constexpr static FloatType epsilon = 0.0000001;
    };
    
    inline FloatType inverseSqrt(FloatType in) {
        return 1.0 / std::sqrt(in);
    }
    
    inline FloatType degToRad(FloatType deg) {
        return deg * (M_PI) / 180.0;
    }
}
