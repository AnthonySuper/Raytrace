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

        class RowSlice {
        public:
            RowSlice(Image* img, size_t h);
            PixelList::iterator begin();
            PixelList::iterator end();
            Vec4& at(size_t w);
            size_t size() const;
        private:
            Image* img;
            size_t h;
        };

        class ConstRowSlice {
        public:
            ConstRowSlice(const Image *, size_t);
            PixelList::const_iterator begin() const;
            PixelList::const_iterator end() const;
            size_t size() const;
        protected:
            const Image * const img;
            size_t h;
        };

        RowSlice at(size_t h);
        ConstRowSlice at(size_t h) const;
        std::vector<RowSlice> rows();
        std::vector<ConstRowSlice> rows() const;       
        inline PixelList::iterator begin() { return pixels.begin(); }
        inline PixelList::iterator end() { return pixels.end(); }
        inline PixelList::const_iterator begin() const {
            return pixels.begin();
        }
        inline PixelList::const_iterator end() const {
            return pixels.end();
        }

        

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
