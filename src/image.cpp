#include <image.hpp>
#include <iostream>

namespace NM {
    Image::Image(size_t height, size_t width) :
        height(height), width(width), pixels(height * width) {}

    Vec4& Image::at(size_t h, size_t w) {
        return pixels.at((h * width) + w);
    }

    Vec4 Image::at(size_t h, size_t w) const {
        return pixels.at((h * width) + w);
    }

    Image::PixelList& Image::getPixels() {
        return pixels;
    }

    void Image::writePPM(std::ostream& os) {
        std::cout << "WritePPM" << std::endl;
        os << "P3" << std::endl;
        os << width << " " << height << " 255" << std::endl;
        
        int alongRow = 0;
        for(const auto& v: pixels) {
            if(v.r() > 255 || v.g() > 255 || v.b() > 255) {
                os << "0 0 0";
            }
            else for(int k = 0; k < 3; ++k) {
                os << static_cast<int>(v[k]);
                if(k != 2) 
                    os << " ";
            }
            if(++alongRow % width) {
                os << " ";
            }
            else {
                os << std::endl;
            }
        }

    }
};
