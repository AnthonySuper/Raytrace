#include <catch.hpp>
#include <vec4.hpp>
#include <camera.hpp>
#include <iostream>

using NM::Camera;
using NM::FloatType;
using NM::Vec4;

TEST_CASE("Basic forward-facing 1x1 camera") {
    FloatType bounds[] = {-1.0, 1.0};
    Camera cam{
        {0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        -1.0,
        bounds,
        boundsq
    };
    
    SECTION("It has the right pos") {
        Vec4 exp{0, 0, 0};
        REQUIRE(cam.pos == exp);
        REQUIRE(cam.getPos() == exp);
    }
}
