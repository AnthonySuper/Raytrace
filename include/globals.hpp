#pragma once

#include <cmath>
#include <iostream>



namespace NM {
	using FloatType = double_t;
    
    struct Constants {
        constexpr static FloatType epsilon = 0.00000001;
    };
    
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
    
    inline constexpr bool fuzzyCompare(FloatType a,
                                       FloatType b,
                                       FloatType epsilon = 0.0001) {
        return constexprAbs(a - b) < epsilon;
    }

    inline void outputProgress(std::string tag, size_t start, size_t end) {
        double percent = (static_cast<double>(start) / end) * 100.0;
        std::cout.precision(4);
        std::cout << std::fixed;
        std::cout << "\r" << tag << "\t" << percent << "\t(";
        std::cout << start << "/" << end << ")";
        std::cout.flush();
    }
}
