cmake_minimum_required(VERSION 3.16)
project(TicketSystem_2021)

set(CMAKE_CXX_STANDARD 20)

include_directories(src/)

add_executable(TicketSystem_2021
        src/main.cpp)


add_subdirectory(src/Tools)
add_subdirectory(src/BPlusTree)
target_link_libraries(TicketSystem_2021 PUBLIC BPlusTree Tools)
