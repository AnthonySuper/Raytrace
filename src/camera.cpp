#include <camera.hpp>

namespace NM {

    CameraAxis::CameraAxis(Vec4 fpos,
                   Vec4 lookAt,
                   Vec4 up) :
        pos(fpos),
        up(up)
        {
            w = (fpos - lookAt).toUnit();
            u = up.cross(w).toUnit();
            v = w.cross(u);
        }
    
    std::ostream& operator<<(std::ostream& os, const CameraAxis& c) {
        return os << "{CameraAxis: Pos=" << c.pos << ", w=" << c.w <<\
            ", v=" << c.v << ", u=" << c.u << "}";
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
    
}
