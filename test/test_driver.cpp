#include <catch.hpp>
#include <driver.hpp>

TEST_CASE("Drive::DriverTransform string reading") {
    NM::Driver::DriverTransform dt;
    std::istringstream is("1 1 1 90 1 0 0 0");
    is >> dt;
    
    SECTION("the translation is zero") {
        NM::Vec4 exp{0, 0, 0};
        REQUIRE(dt.translate == exp);
    }
    
    SECTION("The scale is 1") {
        REQUIRE(dt.scale == 1.0);
    }
    
    SECTION("The angle is properly translated to radians") {
        NM::FloatType exp = NM::degToRad(90);
        REQUIRE(dt.angle == exp);
    }
    
    SECTION("The axis is proper") {
        NM::Vec4 exp{1, 1, 1};
        REQUIRE(dt.axis == exp);
    }
}

TEST_CASE("DriverTransform matrix reading") {
    NM::Driver::DriverTransform dt;
    dt.scale = 1.0;
    dt.axis = {0, 0, 1};
    dt.translate = {0, 1, 1};
    dt.angle = NM::degToRad(90);
    
    SECTION("It has the right rotation matrix") {
        REQUIRE(dt.rotationMatrix() == NM::Transform::axisAngle(dt.axis, dt.angle));
    }
    
    SECTION("It has the right scale transformation") {
        REQUIRE(dt.scaleMatrix() == NM::Transform::scale(dt.scale));
    }
    
    SECTION("It has the right translation transformation") {
        REQUIRE(dt.translationMatrix() == NM::Transform::translate({0, 1, 1}));
    }
    
    SECTION("It has the right combo transformation") {
        auto exp = NM::Transform::translate({0, 1, 1}) *
        NM::Transform::scale(1) *
        NM::Transform::axisAngle(dt.axis, dt.angle);
        REQUIRE(dt.toMatrix() == exp);
    }
}
