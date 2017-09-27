#include <camera.hpp>
#include <iostream>
namespace NM {

    Camera::Camera(Vec4 fpos,
                   Vec4 lookAt,
                   FloatType inear,
                   FloatType horizBounds[2],
                   FloatType vertBounds[2],
                   Vec4 up) :
        pos(fpos),
        up(up),
        near(std::abs(inear)),
        horizontalBounds{horizBounds[0], horizBounds[1]},
        verticalBounds{vertBounds[0], vertBounds[1]} {
            w = (lookAt - fpos).toUnit();
            u = up.cross(w).toUnit();
            v = w.cross(u);
        }
    
    std::vector<Ray> Camera::getRays(size_t height, size_t width) {
        std::vector<Ray> toRet;
        toRet.reserve(height * width);
        for(int i = 0; i < height; ++i) {
            for(int j = 0; j < width; ++j) {
                toRet.emplace_back(getRay(height, width, i, j));
            }
        }
        return toRet;
    }
    
    Ray Camera::getRay(size_t height, size_t width, size_t i, size_t j) {
        float px = i/(width-1.0)*(horizontalBounds[1] - horizontalBounds[0]) + horizontalBounds[0];
        float py = j/(height-1.0)*(verticalBounds[1] - verticalBounds[0]) + verticalBounds[0];
        Vec4 pt = pos + (near * w) + (px * u) + (py * v);
        Vec4 dir = (pt - pos);
        return {pt, dir};
    }
    
    std::ostream& operator<<(std::ostream& os, const Camera& c) {
        return os << "{Camera: Pos=" << c.pos << ", w=" << c.w <<\
            ", v=" << c.v << ", u=" << c.u << "}";
    }
}
