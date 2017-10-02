#include <catch.hpp>
#include <triangle.hpp>
#include <cmath>
#include <transforms.hpp>

using NM::Triangle;
using NM::Vec4;
using NM::Ray;

TEST_CASE("Triangles calculate normals properly") {
    Triangle t{
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0}
    };
    REQUIRE(t.normal.x() == Approx(0.0));
    REQUIRE(t.normal.y() == Approx(0.0));
    REQUIRE(std::abs(t.normal.z()) == Approx(1.0));
}

TEST_CASE("Triangles intersection calculation") {
    Ray cast{{0, 0, 0}, {1, 0, 0}};
    SECTION("With a non-intersecting triangle") {
        Triangle t{
            {-10, 0, 0},
            {-10, 1, 0},
            {-10, 0, 1}
        };
        REQUIRE(t.checkIntersection(cast) == false);
    }
    
    SECTION("With an intersecting triangle") {
        Triangle t{
            {1, 1, 1},
            {1, -1, 0},
            {1, -1, -1}
        };
        auto res = t.checkIntersection(cast);
        REQUIRE(res == true);
        Vec4 exp{1, 0, 0};
        REQUIRE(res.point() == exp);
    }
}

TEST_CASE("Triangle transformation") {
    Triangle tr{
        {0, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    SECTION("With the identity") {
        REQUIRE(NM::Mat4::id().apply(tr) == tr);
    }
    
    SECTION("With a basic translation up z") {
        NM::Mat4 mat = NM::Transform::translate({0, 0, 1});
        Triangle exp{
            {0, 0, 1},
            {0, 1, 1},
            {0, 0, 2}
        };
        REQUIRE(mat.apply(tr) == exp);
    }
}
