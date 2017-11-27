cmake_minimum_required(VERSION 3.2)

project(raytrace)

enable_testing()

set(PROJECT_SRCS
  src/vec4.cpp
  src/mat4.cpp
  src/transforms.cpp
  src/model.cpp
  src/errors.cpp
  src/driver.cpp
  src/file_utils.cpp
  src/ray.cpp
  src/scene.cpp
  src/camera.cpp
  src/ray_intersection.cpp
  src/image.cpp
  src/transformed_drawable.cpp
  src/sphere.cpp
  src/triangle.cpp
  src/material.cpp
  src/wavefront_material_parser.cpp
  src/light.cpp
  src/color.cpp
  src/drawable.cpp)

set(PROJECT_INCLUDES
  include/vec4.hpp
  include/mat4.hpp
  include/transforms.hpp
  include/globals.hpp
  include/model.hpp
  include/camera.hpp
  include/errors.hpp
  include/ray.hpp
  include/driver.hpp
  include/scene.hpp
  include/file_utils.hpp
  include/ray_intersection.hpp
  include/image.hpp
  include/transformed_drawable.hpp
  include/sphere.hpp
  include/triangle.hpp
  include/material.hpp
  include/wavefront_material_parser.hpp
  include/drawable.hpp
  include/light.hpp
  include/color.hpp)

set(PROJECT_TEST_SRCS
  test/test_vec4.cpp
  test/test_mat4.cpp
  test/test_transforms.cpp
  test/test_image.cpp
  test/test_driver.cpp
  test/test_triangle.cpp
  test/test_scene.cpp
  test/test_sphere.cpp
  test/test_material_parser.cpp
  test/test_camera.cpp)

add_test(unit_tests ./catch_test)

add_executable(catch_test test/main.cpp
  ${PROJECT_SRCS}
  ${PROJECT_TEST_SRCS}
  ${PROJECT_INCLUDES})
source_group(includes FILES ${PROJECT_INCLUDES})

source_group(sources FILES ${PROJECT_SRCS})

source_group(test_sources FILES ${PROJECT_TEST_SRCS})

add_executable(raytrace main.cpp
  ${PROJECT_SRCS}
  ${PROJECT_INCLUDES})

target_link_libraries(raytrace pthread)
target_link_libraries(catch_test pthread)

target_include_directories(catch_test PUBLIC include/)
target_include_directories(catch_test PUBLIC lib/)
target_include_directories(raytrace PUBLIC include/)

set(CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(TARGET catch_test PROPERTY CXX_STANDARD 14)
set_property(TARGET raytrace PROPERTY CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-std=c++1z")
