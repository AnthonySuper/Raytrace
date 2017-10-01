#include <mat4.hpp>
#include <vec4.hpp>
#include <catch.hpp>

using namespace NM;

TEST_CASE("Mat4 is the right size") {
    static_assert(sizeof(Mat4) == 4*4*sizeof(FloatType),
            "The matrix must be the right size");
}

TEST_CASE("Mat4 constructs with arrays") {
    FloatType row[] = {0.0, 1.0, 0.0, 0.0};
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

TEST_CASE("Mat4 * Vec4 works arbitrarily") {
    Mat4 mat{1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    Vec4 vec{1, 2, 3};
    Vec4 expect{18, 46, 74, 102};
    REQUIRE((mat * vec) == expect);
}

TEST_CASE("Mat4 identity inverse is the identity") {
    Mat4 inv = Mat4::id();
    REQUIRE(inv.inverse() == inv);
}

TEST_CASE("Mat4 identity inverse works with a random matrix") {
    Mat4 example = {
        2, 3, 1, 5,
        1, 0, 3, 1,
        0, 2, -3, 1,
        0, 2, 3, 1
    };
    Mat4 expected = {
        - 0.666666666666666, 2.333333333333333, 1.5555555555555, -0.5555555555555,
        - 0.3333333333333333, 0.66666666666666, 0.7777777777777, 0.22222222222222,
        -0.0, -0.0, -0.1666666666666666, 0.166666666666666666,
        0.666666666666, -1.3333333333333333, -1.05555555555555, 0.0555555555555555
    };
    Mat4 res = example * example.inverse();
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            if(i == j) {
                REQUIRE(res[i][j] == Approx(1.0));
            }
            else {
                REQUIRE(res[i][j] == Approx(0.0));
            }
        }
    }
    
}
