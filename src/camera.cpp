#include <camera.hpp>

namespace NM {

    Camera::Camera(Vec4 fpos, Vec4 lookAt, FloatType inear, Vec4 up) :
        pos(fpos), up(up), near(inear) {
            w = (fpos - lookAt).toUnit();
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
        double left = width / 2;
        double right = -left;
        double top = height/2;
        double bottom = -top;
        float px = i/(width-1.0)*(right - left) + left;
        float py = j/(height-1.0)*(top - bottom) + bottom;
        Vec4 pt = pos + (near * w) + (px * u) + (py * v);
        Vec4 dir = (pt - pos);
        return {pt, dir};
    }
    
    std::ostream& operator<<(std::ostream& os, const Camera& c) {
        return os << "{Camera: Pos=" << c.pos << ", w=" << c.w <<\
            ", v=" << c.v << ", u=" << c.u << "}";
    }
}
