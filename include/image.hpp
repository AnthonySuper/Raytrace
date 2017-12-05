#pragma once
#include <vec4.hpp>
#include <vector>
#include <stdexcept>
#include <ostream>

// Forward declare CImg
namespace cimg_library {
    template<typename T>
    struct CImg;
}

namespace NM {
    class Image {
    public:
        using PixelList = std::vector<Vec4>;
        const size_t height;
        const size_t width;

        Image(size_t height, size_t width);

        Vec4& at(size_t h, size_t w);
        Vec4 at(size_t h, size_t w) const;
        PixelList& getPixels();
        void writePPM(std::ostream &is);
        void writeFile(std::string fname);

        std::unique_ptr<cimg_library::CImg<unsigned char>> getCImg();
    private:
        PixelList pixels;
    };
}
