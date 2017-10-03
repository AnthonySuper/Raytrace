#pragma once
#include <vec4.hpp>
#include <ray.hpp>
#include <globals.hpp>
#include <vector>
#include <ostream>

namespace NM {
    
    class CameraAxis {
    public:
        CameraAxis(Vec4 point,
               Vec4 lookAt,
               Vec4 up = {0, 0, 1});

        inline const Vec4& getPos() const { return pos; }
        inline const Vec4& getUp() const { return up; }
        inline const Vec4& getW() const { return w; }
        inline const Vec4& getV() const { return v; }
        inline const Vec4& getU() const { return u; }
    private:
        Vec4 pos;
        Vec4 up;
        Vec4 w;
        Vec4 v;
        Vec4 u;
        friend std::ostream& operator<<(std::ostream&, const CameraAxis&);
        
    };
    
    struct CameraAperature {
        FloatType distance;
        FloatType left;
        FloatType right;
        FloatType up;
        FloatType down;
        CameraAperature() = default;
        FloatType getWidthBound(size_t, size_t) const;
        FloatType getHeightBound(size_t, size_t) const;
    };
    
    class Camera {
    public:
        Camera(const CameraAxis &, const CameraAperature&);
        inline CameraAxis getAxis() const { return axis; }
        inline CameraAperature getAperature() const { return aperature; }
        Ray getRay(size_t i, size_t j, size_t height, size_t width) const;
        std::vector<Ray> getRays(size_t height, size_t width) const;
    private:
        CameraAxis axis;
        CameraAperature aperature;
    };
    
    std::ostream& operator<<(std::ostream&, const CameraAxis&);
    std::ostream& operator<<(std::ostream&, const CameraAperature&);
    std::ostream& operator<<(std::ostream&, const Camera&);
}
