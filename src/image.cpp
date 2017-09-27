#include <image.hpp>

namespace NM {
    Image::Image(size_t height, size_t width) :
        height(height), width(width), pixels(height * width) {}

    Vec4& Image::at(size_t h, size_t w) {
        return pixels.at((h * height) + w);
    }

    Vec4 Image::at(size_t h, size_t w) const {
        return pixels.at((h * width) + w);
    }
    
    void Image::writePPM(std::ostream & os, size_t colorDepth) const {
        // write header
        os << "P3" << std::endl;
        // write size
        os << width << " " << height << std::endl;
        // Write color depth
        os << colorDepth << std::endl;
        for(int i = 0; i < height; ++i) {
            for(int j = 0; j < width; ++j) {
                auto v = at(i, j);
                int r = v.r();
                int g = v.g();
                int b = v.b();
                if(r > colorDepth || g > colorDepth || b > colorDepth) {
                    throw std::out_of_range("Number greater than color depth");
                }
                if(r < 0 || g < 0 || b < 0) {
                    throw std::out_of_range("Number less than zero!");
                }
                os << r << " " << g << " " << b << " ";
            }
            os << std::endl;
        }
    }
};
