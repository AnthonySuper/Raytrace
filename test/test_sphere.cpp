#include <catch.hpp>
#include <sphere.hpp>

using NM::Sphere;

TEST_CASE("Basic ray hitting a basic sphere") {
    Sphere s(0.5,{1, 0, 0});
    NM::Ray r{{0, 0, 0}, {1, 0, 0}};
    auto f = s.checkIntersection(r);
    NM::Vec4 intersectPoint{0.5, 0, 0};
    REQUIRE(f == true);
    REQUIRE(f.point() == intersectPoint);
}

TEST_CASE("Basic ray missing basic sphere") {
    Sphere s(0.5, {1, 0, 0});
    NM::Ray r{{0, 0, 0}, {0, 1, 0}};
    REQUIRE(s.checkIntersection(r) == false);
}

