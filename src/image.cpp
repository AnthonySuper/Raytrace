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
        os << "P3" << "\n";
        os << width << " " << height << " 255" << "\n";
        int alongRow = 0;
        for(const auto& v: pixels) {
            for(int k = 0; k < 3; ++k) {
                int res = std::min(255,
                               std::max(0, static_cast<int>(255 * v[k])));
                if(res > 0) {
                    
                }
                os << res;
                if(k != 2) 
                    os << " ";
            }
            if(++alongRow % width) {
                os << " ";
            }
            else {
                os << "\n";
            }
        }
        os.flush();
    }
};
