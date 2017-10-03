#pragma once
#include <vec4.hpp>
#include <ray.hpp>
#include <globals.hpp>
#include <vector>
#include <ostream>

namespace NM {
    
    /**
     * @brief Represent an axis system for a camrea.
     */
    class CameraAxis {
    public:
        /**
         * Create a new axis.
         * @param point the location of the camera
         * @param lookAt what the camera is focused on
         * @param up the direction that is considered "up" in the world
         */
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
    
    /**
     * @brief The aperature of a camera.
     *
     * The Aperature Information for a camera, including its distance and bounds.
     */
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
    
    /**
     * @brief Represents a camrea and everything about it.
     *
     * Cameras have an axis (position and orientation in space), and
     * an aperature.
     */
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
