cmake_minimum_required(VERSION 3.10)

set(MATHSLIB_SOURCES src/vector3.cpp src/particle.cpp)
set(MATHSLIB_HEADERS include/vector3.hpp include/particle.hpp)

add_library(physicslib ${MATHSLIB_SOURCES} ${MATHSLIB_HEADERS})
target_include_directories(physicslib SYSTEM INTERFACE include)
target_include_directories(physicslib PRIVATE include)