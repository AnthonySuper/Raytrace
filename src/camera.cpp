#include <camera.hpp>

namespace NM {

    CameraAxis::CameraAxis(Vec4 fpos,
                   Vec4 lookAt,
                   Vec4 up) :
        pos(fpos),
        up(up)
        {
            w = (fpos - lookAt).toUnit();
            u = w.cross(up).toUnit();
            v = w.cross(u);
        }
    
    std::ostream& operator<<(std::ostream& os, const CameraAxis& c) {
        return os << "{CameraAxis: Pos=" << c.pos << ", w=" << c.w <<\
            ", v=" << c.v << ", u=" << c.u << "}";
    }
    
    static FloatType getRatio(size_t num, size_t denom) {
        if(num > denom) {
            throw std::out_of_range("Width out of bounds");
        }
        if(denom == 1) return 0;
        return (num / (denom - 1.0));
    }
    
    FloatType CameraAperature::getWidthBound(size_t i, size_t total) const {
        return getRatio(i, total)*(left - right) + right;
    }
    
    FloatType CameraAperature::getHeightBound(size_t j, size_t total) const {
        return getRatio(j, total)*(down - up) + up;
    }
    
    Camera::Camera(const CameraAxis & _axis,
                   const CameraAperature & _aperature) :
    axis(_axis), aperature(_aperature) {}
    
    std::ostream& operator<<(std::ostream& os, const CameraAperature& ca) {
        os << "{CameraAperature: dist=" << ca.distance;
        os << ", left=" << ca.left;
        os << ", right=" << ca.right;
        os << ", up=" << ca.up;
        os << ", down=" << ca.down;
        return os;
    }
    
    std::ostream& operator<<(std::ostream& os, const Camera& cam) {
        os << "{Camera: axis=" << cam.getAxis();
        os << ", aperature=" << cam.getAperature() << "}";
        return os;
    }
    
    Ray Camera::getRay(size_t i, size_t j, size_t height, size_t width) const {
        FloatType px = aperature.getHeightBound(i, height);
        FloatType py = aperature.getWidthBound(j, width);
      
        Vec4 rayPt = axis.getPos() +
        (-aperature.distance * axis.getW()) +
        (px * axis.getV()) +
        (py * axis.getU());
        Vec4 rayDir = (rayPt - axis.getPos()).normalized();
        return {rayPt, rayDir};
    }
    
    std::vector<Ray> Camera::getRays(size_t height, size_t width) const {
        std::vector<Ray> toRet;
        for(size_t i = 0; i < height; ++i) {
            for(size_t j = 0; j < width; ++j) {
                toRet.emplace_back(getRay(i, j, height, width));
            }
        }
        return toRet;
    }

    CameraRayGenerator::CameraRayGenerator(const Camera& c, size_t h, size_t w) :
        rootCamera(c), height(height), width(width)
        {}
    
    CameraRayGenerator::CameraRayGenerator(const Camera& c, const Image& im) :
        rootCamera(c), height(im.height), width(im.width) {}
    
    Ray CameraRayGenerator::getRayForIndex(size_t idx) const {
        size_t h = idx / width;
        size_t w = idx % width;
        return rootCamera.getRay(h, w, height, width);
    }
}
