#include <image.hpp>
#include <iostream>
#include <timer.hpp>
#include <errors.hpp>
#define cimg_display 0
#define cimg_use_png 1
#include <CImg.h>
#include <regex>

static std::string getExt(std::string in) {
    std::regex pattern("\\.(.+)",
            std::regex_constants::extended);
    std::smatch m;
    if(! std::regex_search(in, m, pattern)) {
        throw NM::InvalidFilenameError(in);
    }
    return m[m.size() - 1].str();
}

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

    std::unique_ptr<cimg_library::CImg<unsigned char>> Image::getCImg() {
        std::unique_ptr<cimg_library::CImg<unsigned char>> cimg(
                new cimg_library::CImg<unsigned char>(width, height, 1, 4)
        );
        for(size_t i = 0; i < width; ++i) {
            for(size_t j = 0; j < height; ++j) {
                for(size_t c = 0; c < 3; ++c) {
                    auto ch = at(j, i)[c] * 255;
                    unsigned char r =  std::max(0.0,
                            std::min(255.0, ch));
                    (*cimg)(i, j, 0, c) = r;
                }
            }
        }
        cimg->get_shared_channel(3).fill(255);
        return std::move(cimg);
    }

    void Image::writeFile(std::string fname) {
        auto ext = getExt(fname);
        auto img = getCImg();
        img->save(fname.c_str());
        Timer::report("Wrote image");
    }
};
