cmake_minimum_required(VERSION 3.10)

set(MY_EXE_SOURCES src/main.cpp src/Particle.cpp)
set(MY_EXE_HEADERS include/Particle.hpp)

add_executable(MyExe ${MY_EXE_SOURCES} ${MY_EXE_HEADERS})

target_link_libraries(MyExe PUBLIC glfw glad mathslib)