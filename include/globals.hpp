#pragma once

#include <cmath>
#include <ostream>

namespace NM {
	using FloatType = float;
    
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
    /**
     How is this not in the standard library seriously lul
     */
    template<template <typename, typename> class Container, class V, class A>
    void printContainer(std::ostream& os, Container<V, A> const & cont, std::string delim = ", ") {
        auto begin = std::begin(cont);
        auto end = std::end(cont);
        while(begin != end) {
            os << *begin;
            begin++;
            if(begin != end) {
                os << delim;
            }
        }
    }

}


