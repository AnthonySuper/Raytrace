#include "vec4.hpp"
#include "catch.hpp"

using namespace NM;

TEST_CASE("Vec4 is the right size") {
    static_assert(sizeof(Vec4) == (4 * sizeof(double)),
            "Vector is incorrect size");
}

TEST_CASE("Vec4 construction works with small stuff") {
    Vec4 v(1, 1, 1);
    REQUIRE(v.x() == 1);
    REQUIRE(v.y() == 1);
    REQUIRE(v.z() == 1);
}

TEST_CASE("Indexing works") {
    Vec4 v(1, 1, 1);
    REQUIRE(v[0] == 1);
    v[0] = 20.0;
    REQUIRE(v.x() == 20);
}

TEST_CASE("Test operator ==") {
    Vec4 a(1, 2, 3);
    Vec4 b(1, 1, 1);
    Vec4 c(1, 1, 1);
    REQUIRE(a != b);
    REQUIRE(b != a);
    REQUIRE(b == c);
    REQUIRE(c == b);
    REQUIRE(c == c);
}

TEST_CASE("Test operator +=") {
    Vec4 a(1, 1, 1);
    a += a;
    Vec4 expect(2, 2, 2);
    REQUIRE(a == expect);
}

TEST_CASE("Test operator -=") {
    Vec4 a(1, 1, 1);
    Vec4 res(0, 0, 0);
    a -= a;
    REQUIRE(a == res);
}

TEST_CASE("Test operator *= with int") {
    int i = 2;
    Vec4 a(1, 1, 1);
    a *= 2;
    Vec4 exp(2, 2, 2);
    REQUIRE(a == exp);
}

TEST_CASE("Test operator +") {
    Vec4 a(1, 1, 1);
    Vec4 b(1, 2, 1);
    Vec4 res(2, 3, 2);
    REQUIRE((a + b) == res);
}

TEST_CASE("Test operator -") {
    Vec4 a(1, 1, 1);
    Vec4 b(1, 2, 1);
    Vec4 res{0, 1, 0};
    REQUIRE((b - a) == res);
}

TEST_CASE("Test operator *") {
    Vec4 a(1, 2, 1);
    Vec4 res(2, 4, 2);
    REQUIRE((a * 2) == res);
    REQUIRE((a * 2.0) == res);
    REQUIRE((2.0 * a) == res);
}
