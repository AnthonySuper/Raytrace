#include "vec4.hpp"
#include "catch.hpp"
#include <sstream>

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
    Vec4 expect{2, 2, 2, 2};
    REQUIRE(a == expect);
}

TEST_CASE("Test operator -=") {
    Vec4 a(1, 1, 1);
    Vec4 res(0, 0, 0, 0);
    a -= a;
    REQUIRE(a == res);
}

TEST_CASE("Test operator *= with int") {
    int i = 2;
    Vec4 a(1, 1, 1);
    a *= 2;
    Vec4 exp(2, 2, 2, 2);
    REQUIRE(a == exp);
}

TEST_CASE("Test operator +") {
    Vec4 a(1, 1, 1);
    Vec4 b(1, 2, 1);
    Vec4 res{2, 3, 2, 2};
    REQUIRE((a + b) == res);
}

TEST_CASE("Test operator -") {
    Vec4 a(1, 1, 1);
    Vec4 b(1, 2, 1);
    Vec4 res{0, 1, 0, 0};
    REQUIRE((b - a) == res);
}

TEST_CASE("Test operator *") {
    Vec4 a(1, 2, 1);
    Vec4 res(2, 4, 2, 2);
    REQUIRE((a * 2) == res);
    REQUIRE((a * 2.0) == res);
    REQUIRE((2.0 * a) == res);
}

TEST_CASE("Test manitude") {
    Vec4 easy(1, 0, 0);
    REQUIRE(easy.magnitude() == 1.0);
    Vec4 harder(2, 2, 2);
    REQUIRE(harder.magnitude() == Approx(3.46410161514));
}

TEST_CASE("Vec4 can be read via streams without w") {
    Vec4 toRead{0, 0, 0};
    Vec4 expect{1, 3, 4};
    std::string str("1 3 4");
    std::istringstream ios(str);
    ios >> toRead;
    REQUIRE(toRead == expect);
}

TEST_CASE("Vec4 can be read via streams with w") {
    Vec4 toRead{0, 0, 0};
    Vec4 expect{1, 3, 4, 5};
    std::istringstream ios("1 3 4 5");
    ios >> toRead;
    REQUIRE(toRead == expect);
}

TEST_CASE("Vec4 throws when reading from a bad stream") {
    Vec4 toRead{0, 0, 0};
    std::istringstream ios("1 garbage 2 3");
    REQUIRE_THROWS(ios >> toRead);
}
