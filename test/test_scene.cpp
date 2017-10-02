#include <catch.hpp>
#include <scene.hpp>
#include <sphere.hpp>

using NM::Sphere;
using NM::Scene;
using NM::Ray;

TEST_CASE("With a two-sphere scene") {
    Scene s;
    s.addObject(Sphere{0.5, {0, 0, 1}});
    s.addObject(Sphere{0.5, {0, 0, 2}});
    
    SECTION("With a ray that misses returns a miss") {
        Ray r{{0, 0, 0}, {1, 0, 0}};
        REQUIRE(! s.trace(r));
    }
    
    SECTION("With a ray that hits") {
        Ray r{{0, 0, 0}, {0, 0, 1}};
        auto res = s.trace(r);
        SECTION("It hits") {
            REQUIRE(res.operator bool());
        }
        
        SECTION("Is has the right distance") {
            REQUIRE(res.getDistance() == Approx(0.5));
        }
    }
}
