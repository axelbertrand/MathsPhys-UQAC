cmake_minimum_required(VERSION 3.10)

set(PHYSICSLIB_SOURCES src/vector3.cpp src/particle.cpp src/gravityForceGenerator.cpp src/dragForceGenerator.cpp src/particleSpringForceGenerator.cpp src/anchoredSpringForceGenerator.cpp src/bungeeSpringForceGenerator.cpp src/particleBuoyancyForceGenerator.cpp src/particleStiffSpringForceGenerator.cpp src/forceRegister.cpp src/particleLink.cpp src/particleCable.cpp src/particleRod.cpp src/particleContact.cpp src/particleContactResolver.cpp src/contactRegister.cpp src/blob.cpp)
set(PHYSICSLIB_HEADERS include/vector3.hpp include/particle.hpp include/particleForceGenerator.hpp include/gravityForceGenerator.hpp include/dragForceGenerator.hpp include/particleSpringForceGenerator.hpp include/anchoredSpringForceGenerator.hpp include/bungeeSpringForceGenerator.hpp include/particleBuoyancyForceGenerator.hpp include/particleStiffSpringForceGenerator.hpp include/forceRegister.hpp include/particleContact.hpp include/particleContactGenerator.hpp include/particleContactResolver.hpp include/particleLink.hpp include/particleCable.hpp include/particleRod.hpp include/contactRegister.hpp include/blob.hpp)


add_library(physicslib ${PHYSICSLIB_SOURCES} ${PHYSICSLIB_HEADERS})
target_include_directories(physicslib SYSTEM INTERFACE include)
target_include_directories(physicslib PRIVATE include)