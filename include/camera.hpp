#pragma once
#include <vec4.hpp>
#include <ray.hpp>
#include <globals.hpp>
#include <vector>
#include <ostream>
#include <cmath>

namespace NM {
    
    class Camera {
    public:
        Camera() = default;
        Camera(Vec4 point,
               Vec4 lookAt,
               FloatType near,
               FloatType horizBounds[2],
               FloatType vertBounds[2],
               Vec4 up = {0, 1, 0});
        
        std::vector<Ray> getRays(size_t height, size_t width);
        
        Ray getRay(size_t height, size_t width, size_t i, size_t j);
        
        Vec4 getPos() const { return pos; }
        Vec4 pos;
        Vec4 up;
        Vec4 w;
        Vec4 v;
        Vec4 u;
        FloatType near;
        FloatType horizontalBounds[2] ;
        FloatType verticalBounds[2];
    };
    
    std::ostream& operator<<(std::ostream&, const Camera&);
}
