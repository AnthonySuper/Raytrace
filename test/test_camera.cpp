#include <catch.hpp>
#include <camera.hpp>

TEST_CASE("A very basic camera") {
    NM::CameraAxis axis{
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0}
    };

    NM::CameraAperature ap{
        5, -2, 2, 2, -2
    };

    NM::Camera cam(axis, ap);

    SECTION("Ray generators work as expected") {
        NM::Image im{10, 10};
        auto g = cam.rayGeneratorFor(im);
        auto v = g.toVector();
        auto exp = cam.getRays(10, 10);
        REQUIRE(v == exp);
    }
}