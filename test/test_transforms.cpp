#include <mat4.hpp>
#include <vec4.hpp>
#include <transforms.hpp>
#include <catch.hpp>


using namespace NM;

TEST_CASE("Scaling by identity", "[scaling]") {
    auto mat = Transform::scale({1, 1, 1});
    auto testVec = Vec4{5, 6, 7};
    REQUIRE((mat * testVec) == testVec);
}

TEST_CASE("Scaling equally in all direction via constant", "[scaling]") {
    auto mat = Transform::scale(2);
    auto testVec = Vec4{2, 3, 4};
    Vec4 expect{4, 6, 8};
    REQUIRE((mat * testVec) == expect);
}

TEST_CASE("Scaling by a scalar identity", "[scaling]") {
    auto mat = Transform::scale(1);
    auto testVec = Vec4{5,6,7};
    REQUIRE(mat(testVec) == testVec);
}

TEST_CASE("Scaling inequally in directions", "[scaling]") {
    auto mat = Transform::scale({2, 1, 3});
    auto testVec = Vec4{2, 3, 4};
    auto expect = Vec4{4, 3, 12};
    REQUIRE((mat * testVec) == expect);
}

TEST_CASE("Translation in one direction", "[translation]") {
    auto mat = Transform::translate({5, 0, 0});
    auto testVec = Vec4{3, 2, 1};
    Vec4 expect{8, 2, 1};
    REQUIRE((mat * testVec) == expect);
}

TEST_CASE("Translation in many directions", "[translation]") {
    auto mat = Transform::translate({5, 5, 5});
    Vec4 test{1, 1, 1};
    Vec4 expect{6, 6, 6};
    REQUIRE((mat * test) == expect);
}

TEST_CASE("Transform about the Z axis", "[transform]") {
    auto mat = Transform::axisAngle({0, 0, 1}, M_PI);
    SECTION("The matrix is right according to roateZ") {
        auto expect = Transform::rotateZ(M_PI);
        REQUIRE(mat == expect);
    }
    
    SECTION("The matrix is right according to the internet") {
        Mat4 real{-1.0, 0.0, 0.0, 0,
            0, -1.0, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        REQUIRE(mat.truncedZeros() == real);
    }
    
    SECTION("Against vectors") {
        Vec4 in{1, 0, 0};
        Vec4 exp{-1, 0, 0};
        REQUIRE(mat(in).fuzzyEquals(exp));
    }
}

TEST_CASE("P1 failures for Driver04", "[transform]") {
    auto rad = NM::degToRad(-90);
    Mat4 rotate = Transform::axisAngle({0, 0, 10}, rad);
    Mat4 scale = Transform::scale(1.0);
    Mat4 translate = Transform::translate({2, 2, 0});
    Mat4 app = translate * scale * rotate;
    SECTION("Vertex 1") {
        Vec4 exp{2, 1, 0};
        Vec4 in{1, 0, 0};
        REQUIRE(exp == app(in));
    }
    SECTION("Vertex 2") {
    }
}
