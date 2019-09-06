cmake_minimum_required(VERSION 3.10)

set(MY_LIB_SOURCES src/my_lib.cpp)
set(MY_LIB_HEADERS include/my_lib.hpp)

add_library(MyLib ${MY_LIB_SOURCES} ${MY_LIB_HEADERS})
target_include_directories(MyLib SYSTEM INTERFACE include)
target_include_directories(MyLib PRIVATE include)