
add_library(Tools Algorithm.hpp)

add_executable(ToolsTest test.cpp)
target_link_libraries(ToolsTest PUBLIC Tools)