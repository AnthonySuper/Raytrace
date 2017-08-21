#include <mat4.hpp>
#include <vec4.hpp>
#include <catch.hpp>

using namespace NM;

TEST_CASE("Mat4 is the right size") {
    static_assert(sizeof(Mat4) == 4*4*sizeof(Mat4::ItemType),
            "The matrix must be the right size");
}

TEST_CASE("Mat4 constructs with arrays") {
    double row[] = {0.0, 1.0, 0.0, 0.0};
    Mat4 mat{row, row, row, row};
    REQUIRE(mat[0][1] == 1.0);
    REQUIRE(mat[0][2] == 0.0);
}

TEST_CASE("Mat4 fakes being an aggregate") {
    Mat4 mat{1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};
    REQUIRE(mat[0][0] == 1);
}

TEST_CASE("Mat4 can be constructed with row vectors") {
    Vec4 vec{1, 0, 0};
    Mat4 mat(vec, vec, vec, vec);
    REQUIRE(mat[0][0] == 1);
}

TEST_CASE("Mat4 Multiplication by the id works") {
    Mat4 m{0, 0, 0, 1,
        0, 2, 0, 0,
        0, 0, 3, 0, 
        1, 0, 0, 0};
    Mat4 id = Mat4::id();
    REQUIRE((m * id) == m);
}

TEST_CASE("Mat4 * Vec4 by id works") {
    Vec4 vec(1, 3, 4);
    Mat4 id = Mat4::id();
    REQUIRE((id * vec) == vec);
}
