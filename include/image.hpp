#pragma once
#include <vec4.hpp>
#include <vector>
#include <stdexcept>

namespace NM {
    class Image {
    public:
        using PixelList = std::vector<Vec4>;
        const size_t height;
        const size_t width;
        Image(size_t height, size_t width);
        Vec4& at(size_t h, size_t w);
        Vec4 at(size_t h, size_t w) const;
        inline Vec4& at(size_t idx) { return pixels.at(idx); }
        inline const Vec4& at(size_t idx) const { return pixels.at(idx); }
        inline PixelList::iterator begin() { return pixels.begin(); }
        inline PixelList::iterator end() { return pixels.end(); }
        inline PixelList::const_iterator begin() const {
            return pixels.begin();
        }
        inline PixelList::const_iterator end() const {
            return pixels.end();
        }
        void writePPM(std::ostream&, size_t colorDepth = 255) const;

    private:
        PixelList pixels;
    };

    template<typename T>
    void each_pixel(NM::Image& img, T f) {
        for(size_t i = 0; i < img.height; ++i) {
            for(size_t j = 0; j < img.width; ++i) {
                f(i, j, img.at(i, j));
            }
        }
    }

    template<class OutputIt, class UnaryOperation>
    OutputIt transform_pixel(Image img, OutputIt first, UnaryOperation op) {
        for(size_t i = 0; i < img.height; ++i) {
            for(size_t j = 0; j < img.width; ++j) {
                *first = op(i, j, img.at(i, j));
                ++first;
            }
        }
        return first;
    };
}
