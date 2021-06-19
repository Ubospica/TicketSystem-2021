cmake_minimum_required(VERSION 3.16)

SET(CMAKE_C_COMPILER /usr/bin/clang)
SET(CMAKE_CXX_COMPILER /usr/bin/clang++)


project(code)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Ofast)

include_directories(src/)
include_directories(src/Tools)
include_directories(src/BPlusTree)

add_executable(code src/Test_Train/hpp/main.cpp)
#add_executable(remake src/remake/main.cpp)

add_subdirectory(src/Tools)
add_subdirectory(src/BPlusTree)
add_subdirectory(src/Test_Train/hpp)
