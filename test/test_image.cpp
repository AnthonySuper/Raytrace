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
