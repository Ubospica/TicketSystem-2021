cmake_minimum_required(VERSION 3.16)
project(TicketSystem_2021)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Ofast)

include_directories(src/)
include_directories(src/Tools)
include_directories(src/BPlusTree)


add_subdirectory(src/Tools)
add_subdirectory(src/BPlusTree)
add_subdirectory(src/Test_Train/hpp)
