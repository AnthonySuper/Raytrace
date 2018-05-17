#include <catch.hpp>
#include <scene.hpp>
#include <sphere.hpp>


using NM::Sphere;
using NM::Scene;
using NM::Ray;
using NM::RayResult;

TEST_CASE("With a two-sphere scene") {
    /* TODO: FIX
    Scene s;
    s.addObject(Sphere{0.5, {0, 0, 1}, nullptr});
    s.addObject(Sphere{0.5, {0, 0, 2}, nullptr});
    
    SECTION("With a ray that misses returns a miss") {
        RayResult r{{{0, 0, 0}, {1, 0, 0}}};
        s.intersect(r);
        REQUIRE(! r);
    }
    
    
    SECTION("With a ray that hits") {
        RayResult r{{{0, 0, 0}, {0, 0, 1}}};
        s.intersect(r);
        SECTION("It hits") {
            REQUIRE(r.operator bool());
        }
        
        SECTION("Is has the right distance") {
            REQUIRE(r.distance == Approx(0.5));
        }
    }
    */
}
