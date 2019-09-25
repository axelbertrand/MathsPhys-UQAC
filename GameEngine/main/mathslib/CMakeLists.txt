cmake_minimum_required(VERSION 3.10)

set(MATHSLIB_SOURCES src/vector3.cpp src/ParticleForceGenerator.cpp src/GravityForceGenerator.cpp src/DragForceGenerator.cpp src/ForceRegister.cpp)
set(MATHSLIB_HEADERS include/vector3.hpp include/ParticleForceGenerator.hpp include/GravityForceGenerator.hpp include/DragForceGenerator.hpp include/ForceRegister.hpp)

add_library(mathslib ${MATHSLIB_SOURCES} ${MATHSLIB_HEADERS})
target_include_directories(mathslib SYSTEM INTERFACE include)
target_include_directories(mathslib PRIVATE include)