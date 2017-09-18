#include <catch.hpp>
#include <image.hpp>

using NM::Image;
using NM::Vec4;

TEST_CASE("Setting dimensions", "[image]") {
    Image i(20, 20);
    REQUIRE(i.height == 20);
    REQUIRE(i.width == 20);
}

TEST_CASE("Iterating", "[image]") {
    Image i(10, 10);
    Vec4 exp{1, 1, 1};
    i.at(1, 0) = exp;
    auto r = i.begin();
    REQUIRE(*(r + 10) == exp);
}

TEST_CASE("Row iterating", "[image]") {
    Image i(10, 10);
    REQUIRE(i.rows().size() == 10);
}

TEST_CASE("each_pixel", "[image]") {
    Image i(2, 2);
    i.at(0, 0) = {2, 3, 4};
    i.at(1, 1) = {2, 3, 4};
    int count;
    each_pixel(i, [&](size_t height, size_t width, Vec4 vec) {
            count += vec[2];
            });
    REQUIRE(count == 6);
}
