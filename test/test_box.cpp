#include <catch.hpp>
#include <box.hpp>

using NM::Box;
using NM::Ray;
using NM::RayResult;

TEST_CASE("Box Collisions") {
    Box box{{-1, -1, -1}, {1, 1, 1}};
    
    SECTION("Works with a simple head-on collision in the Y") {
        Ray ray{{0, 10, 0}, {0, -1, 0}};
        RayResult r(ray);
        REQUIRE(box.intersect(r));
    }
    
    SECTION("Works with a head-on collision in the X") {
        Ray ray{{10, 0, 0}, {-1, 0, 0}};
        RayResult r(ray);
        REQUIRE(box.intersect(r));
    }
    
    SECTION("Works with a head-on collision in the Z") {
        Ray ray{{0, 0, 10}, {0, 0, -1}};
        RayResult r(ray);
        REQUIRE(box.intersect(r));
    }
    
    SECTION("With a head-on collision in the Z from below") {
        Ray ray{{0, 0, -10}, {0, 0, 1}};
        RayResult r(ray);
        REQUIRE(box.intersect(r));
    }
    
    SECTION("With a ray that starts inside the box") {
        Ray ray{{0, 0, 0}, {1, 0, 0}};
        RayResult r(ray);
        REQUIRE(box.intersect(r));
    }
    
    SECTION("With a ray that doesn't intersect") {
        Ray ray{{0, 10, 0}, {1, 0, 0}};
        RayResult r(ray);
        REQUIRE(box.intersect(r) == false);
    }
    
    SECTION("With a ray on the point going away") {
        Ray ray{{-1, -1, -1}, {-1, 0, 0}};
        RayResult r(ray);
        REQUIRE(box.intersect(r));
    }
}
