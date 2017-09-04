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
}
