#pragma once
#include <vec4.hpp>
#include <ray.hpp>
#include <globals.hpp>
#include <vector>
#include <ostream>

namespace NM {
    
    class Camera {
    public:
        Camera(Vec4 point, Vec4 lookAt, FloatType near, Vec4 up = {0, 1, 0});
        
        std::vector<Ray> getRays(size_t height, size_t width);
        
        Ray getRay(size_t height, size_t width, size_t i, size_t j);
        
    private:
        Vec4 pos;
        Vec4 up;
        Vec4 w;
        Vec4 v;
        Vec4 u;
        FloatType near;
        friend std::ostream& operator<<(std::ostream&, const Camera&);
        
    };
    
    std::ostream& operator<<(std::ostream&, const Camera&);
}
