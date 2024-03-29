#include <catch.hpp>
#include <sphere.hpp>

using NM::Sphere;
using NM::RayResult;
using NM::Vec4;

TEST_CASE("Basic ray hitting a basic sphere") {
    Sphere s(0.5,{1, 0, 0});
    RayResult r{{{0, 0, 0}, {1, 0, 0}}};
    s.intersects(r);
    Vec4 intersectPoint{0.5, 0, 0};
    REQUIRE(r == true);
    REQUIRE(r.point() == intersectPoint);
}

TEST_CASE("Basic ray missing basic sphere") {
    Sphere s(0.5, {1, 0, 0});
    RayResult r{{{0, 0, 0}, {0, 1, 0}}};
    REQUIRE(s.intersects(r) == false);
}

TEST_CASE("Basic ray with a sphere behind it") {
    Sphere s(0.5, {-1, 0, 0});
    RayResult r{{{0, 0, 0}, {1, 0,0}}};
    auto inter = s.intersects(r);
    REQUIRE(inter == false);
}

TEST_CASE("Surface normals") {
    Sphere s(1, {0, 0, 0});
    
    SECTION("With an easy direct hit") {
        RayResult inter{{
            {2, 0, 0}, {-1, 0, 0}
        }};
        s.intersects(inter);
        Vec4 exp = Vec4{1, 0, 0}.toUnit();
        s.swapInfo(inter);
        REQUIRE(inter.surfaceNormal == exp);
    }
    SECTION("With a hit from above") {
        RayResult inter{{
            {0, 2, 0}, {0, -1, 0}
        }};
        s.intersects(inter);
        Vec4 exp = Vec4{0, 1, 0}.toUnit();
        s.swapInfo(inter);
        REQUIRE(inter.surfaceNormal == exp);
    }
}
