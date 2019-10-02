cmake_minimum_required(VERSION 3.10)

set(PHYSICSLIB_SOURCES src/vector3.cpp src/particle.cpp src/gravityForceGenerator.cpp src/dragForceGenerator.cpp src/forceRegister.cpp)
set(PHYSICSLIB_HEADERS include/vector3.hpp include/particle.hpp include/particleForceGenerator.hpp include/gravityForceGenerator.hpp include/dragForceGenerator.hpp include/forceRegister.hpp)


add_library(physicslib ${PHYSICSLIB_SOURCES} ${PHYSICSLIB_HEADERS})
target_include_directories(physicslib SYSTEM INTERFACE include)
target_include_directories(physicslib PRIVATE include)