#include <image.hpp>

namespace NM {
    Image::Image(size_t height, size_t width) :
        height(height), width(width), pixels(height * width) {}

    Vec4& Image::at(size_t h, size_t w) {
        return pixels.at((h * height) + w);
    }

    Vec4 Image::at(size_t h, size_t w) const {
        return pixels.at((h * height) + w);
    }

    Image::RowSlice Image::at(size_t h) {
        return {this, h};
    }

    Image::ConstRowSlice Image::at(size_t h) const {
        return {this, h};
    }

    Image::RowSlice::RowSlice(Image* img, size_t h) :
        img(img), h(h) {
            if(h > img->height) throw std::out_of_range("Height too large");
        }

    Image::PixelList::iterator Image::RowSlice::begin() {
        return img->begin() + (h * img->height);
    }

    Image::PixelList::iterator Image::RowSlice::end() {
        return begin() + img->width;
    }

    size_t Image::RowSlice::size() const {
        return img->width;
    }

    Image::ConstRowSlice::ConstRowSlice(const Image * const i, size_t h) :
        img(i), h(h) {
            if(h > i->height) throw std::out_of_range("Height too large");
        }

    Image::PixelList::const_iterator Image::ConstRowSlice::begin() const {
        return img->begin() + (h * img->height);
    }

    Image::PixelList::const_iterator Image::ConstRowSlice::end() const {
        return begin() + (h * img->height);
    }

    size_t Image::ConstRowSlice::size() const {
        return img->width;
    }

    std::vector<Image::RowSlice> Image::rows() {
        std::vector<RowSlice> toRet;
        for(size_t i = 0; i < height; ++i) {
            toRet.emplace_back(this, i);
        }
        return toRet;
    }

    std::vector<Image::ConstRowSlice> Image::rows() const {
        std::vector<ConstRowSlice> toRet;
        for(size_t i = 0; i < height; ++i) {
            toRet.emplace_back(this, i);
        }
        return toRet;
    }
};
