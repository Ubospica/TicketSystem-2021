
add_library(Tools String.cpp)
target_include_directories(Tools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ToolsTest test.cpp)
target_link_libraries(ToolsTest PUBLIC Tools)