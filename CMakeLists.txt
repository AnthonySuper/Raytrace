cmake_minimum_required(VERSION 3.2)

project(raytrace)

enable_testing()

set(PROJECT_SRCS)

set(PROJECT_INCLUDES
  include/vec4.hpp
  include/mat4.hpp
  include/transform.hpp
  include/globals.hpp)

set(PROJECT_TEST_SRCS
  test/test_vec4.cpp
  test/test_mat4.cpp)

add_test(unit_tests COMMAND ./catch_test)

add_executable(catch_test test/main.cpp
  ${PROJECT_SRCS}
  ${PROJECT_TEST_SRCS})


target_include_directories(catch_test PUBLIC include/)
target_include_directories(catch_test PUBLIC lib/)

set(CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)