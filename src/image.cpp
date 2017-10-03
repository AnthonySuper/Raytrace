#include <image.hpp>

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
        os << "P3" << std::endl;
        os << width << " " << height << std::endl;
        os << 255 << std::endl;
       ;
        for(int i = 0; i < height; ++i) {
            for(int j = 0; j < width; ++j) {
                auto v = at(i, j);
                if(v.r() > 255 || v.g() > 255 || v.b() > 255) {
                    os << "0 0 0 ";
                    continue;
                }
                for(int i = 0; i < 3; ++i) {
                    os << static_cast<int>(v[i]);
                    os << " ";
                }
            }
            os << std::endl;
        }
    }
};
