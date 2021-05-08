project(PROJET_META)
cmake_minimum_required(VERSION 3.20)

SET(CMAKE_C_COMPILER "cc")
SET(CMAKE_C_FLAGS "-std=c18 -O2")
SET(CMAKE_CXX_COMPILER "c++")
SET(CMAKE_CXX_FLAGS "-std=c++17 -O2")
SET(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -Wextra")
SET(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -O2")
SET(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include /usr/include/python3.9 /home/kodra/IBM/cplex/include /home/kodra/IBM/concert/include)
link_directories(/home/kodra/IBM/cplex/lib/x86-64_linux/static_pic/ /home/kodra/IBM/concert/lib/x86-64_linux/static_pic)

add_executable(meta src/main.cpp src/rwfile.cpp src/draw.cpp src/mip.cpp src/kdtree.c)

target_link_libraries(meta tbb python3.9 m dl concert ilocplex cplex pthread)
