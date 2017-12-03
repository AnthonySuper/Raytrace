cmake_minimum_required(VERSION 3.2)

project(raytrace)

enable_testing()

set(PROJECT_GEOMETRY_SRCS
    src/vec4.cpp
    src/mat4.cpp
    src/transforms.cpp
    src/ray_intersection.cpp
    src/ray.cpp
    src/ray_result.cpp
    src/box.cpp)

set(PROJECT_DRAWABLE_SRCS
    src/drawable.cpp
    src/transformed_drawable.cpp
    src/sphere.cpp
    src/model.cpp
    src/triangle.cpp
    src/face.cpp)

set(PROJECT_UTIL_SRCS
    src/errors.cpp
    src/file_utils.cpp
    src/wavefront_material_parser.cpp
    src/driver.cpp)

set(PROJECT_TRACE_SRCS
    src/camera.cpp
    src/scene.cpp
    src/image.cpp
    src/material.cpp
    src/wavefront_material_parser.cpp
    src/light.cpp
    src/bonsai_tree.cpp)

SET(PROJECT_SRCS
    ${PROJECT_GEOMETRY_SRCS}
    ${PROJECT_DRAWABLE_SRCS}
    ${PROJECT_UTIL_SRCS}
    ${PROJECT_TRACE_SRCS})


set(PROJECT_GEOMETRY_INCLUDES
    include/vec4.hpp
    include/mat4.hpp
    include/transforms.hpp
    include/ray.hpp
    include/ray_intersection.hpp
    include/ray_result.hpp
    include/box.hpp)

SET(PROJECT_DRAWABLE_INCLUDES
    include/drawable.hpp
    include/model.hpp
    include/transformed_drawable.hpp
    include/sphere.hpp
    include/triangle.hpp
    include/face.hpp)

set(PROJECT_UTIL_INCLUDES
    include/globals.hpp
    include/errors.hpp
    include/file_utils.hpp
    include/driver.hpp)

set(PROJECT_TRACE_INCLUDES
    include/camera.hpp
    include/scene.hpp
    include/image.hpp
    include/material.hpp
    include/wavefront_material_parser.hpp
    include/light.hpp
    include/bonsai_tree.hpp)

set(PROJECT_INCLUDES
${PROJECT_GEOMETRY_INCLUDES}
${PROJECT_DRAWABLE_INCLUDES}
${PROJECT_UTIL_INCLUDES}
${PROJECT_TRACE_INCLUDES})


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
  test/test_camera.cpp
  test/test_box.cpp)

add_test(unit_tests ./catch_test)

add_executable(catch_test test/main.cpp
  ${PROJECT_SRCS}
  ${PROJECT_TEST_SRCS}
  ${PROJECT_INCLUDES})

source_group(geometry\\srcs FILES ${PROJECT_GEOMETRY_SRCS})
source_group(geometry\\includes FILES ${PROJECT_GEOMETRY_INCLUDES})

source_group(drawable\\srcs FILES ${PROJECT_DRAWABLE_SRCS})
source_group(drawable\\includes FILES ${PROJECT_DRAWABLE_INCLUDES})

source_group(util\\srcs FILES ${PROJECT_UTIL_SRCS})
source_group(util\\includes FILES ${PROJECT_UTIL_INCLUDES})

source_group(trace\\srcs FILES ${PROJECT_TRACE_SRCS})
source_group(trace\\includes FILES ${PROJECT_TRACE_INCLUDES})

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
