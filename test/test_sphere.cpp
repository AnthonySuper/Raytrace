#include <catch.hpp>
#include <sphere.hpp>

using NM::Sphere;
using NM::Ray;
using NM::Vec4;

TEST_CASE("Basic ray hitting a basic sphere") {
    Sphere s(0.5,{1, 0, 0});
    Ray r{{0, 0, 0}, {1, 0, 0}};
    auto f = s.checkIntersection(r);
    Vec4 intersectPoint{0.5, 0, 0};
    REQUIRE(f == true);
    REQUIRE(f.point() == intersectPoint);
}

TEST_CASE("Basic ray missing basic sphere") {
    Sphere s(0.5, {1, 0, 0});
    Ray r{{0, 0, 0}, {0, 1, 0}};
    REQUIRE(s.checkIntersection(r) == false);
}

TEST_CASE("Basic ray with a sphere behind it") {
    Sphere s(0.5, {-1, 0, 0});
    Ray r({0, 0, 0}, {1, 0,0});
    auto inter = s.checkIntersection(r);
    REQUIRE(inter == false);
}

TEST_CASE("Surface normals") {
    Sphere s(1, {0, 0, 0});
    
    SECTION("With an easy direct hit") {
        auto inter = s.checkIntersection({
            {2, 0, 0}, {-1, 0, 0}
        });
        Vec4 exp = Vec4{1, 0, 0}.toUnit();
        REQUIRE(inter.surfaceNormal() == exp);
    }
    SECTION("With a hit from above") {
        auto inter = s.checkIntersection({
            {0, 2, 0}, {0, -1, 0}
        });
        Vec4 exp = Vec4{0, 1, 0}.toUnit();
        REQUIRE(inter.surfaceNormal() == exp);
    }
}
