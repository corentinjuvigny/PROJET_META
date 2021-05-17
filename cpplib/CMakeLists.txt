project(PROJET_META)
cmake_minimum_required(VERSION 3.20)

SET(CMAKE_C_COMPILER "cc")
SET(CMAKE_C_FLAGS "-std=c18 -O3")
SET(CMAKE_CXX_COMPILER "c++")
SET(CMAKE_CXX_FLAGS "-O3 -DWITH_EIGEN=1")
SET(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -Wextra")
SET(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -O2")
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED 18)

include_directories(${PROJECT_SOURCE_DIR}/include /usr/include/python3.9 /opt/ibm/ILOG/CPLEX_Studio128/cplex/include /opt/ibm/ILOG/CPLEX_Studio128/concert/include /usr/local/include/paradiseo/eo /usr/local/include/paradiseo/mo /usr/local/include/paradiseo/edo /usr/local/include/paradiseo/moeo /usr/include/eigen3)
link_directories(/opt/ibm/ILOG/CPLEX_Studio128/cplex/lib/x86-64_linux/static_pic/ /opt/ibm/ILOG/CPLEX_Studio128/concert/lib/x86-64_linux/static_pic)

add_executable(meta src/main.cpp src/rwfile.cpp src/draw.cpp src/mip.cpp src/kdtree.c)

target_link_libraries(meta tbb python3.9 m dl concert ilocplex cplex pthread)
