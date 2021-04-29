project(PROJET_META)
cmake_minimum_required(VERSION 3.20)

SET(CMAKE_C_COMPILER "cc")
SET(CMAKE_C_FLAGS "-std=c99 -O2")
SET(CMAKE_CXX_COMPILER "c++")
SET(CMAKE_CXX_FLAGS "-std=c++20")
SET(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -Wextra")
SET(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -O2")
SET(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(meta src/main.cpp src/node.cpp src/grid.cpp src/kdtree.c)

target_link_libraries(meta tbb m)
