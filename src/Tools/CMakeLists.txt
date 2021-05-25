
add_library(Tools String.cpp Time.cpp FileIO.cpp HashMap.cpp List.cpp)
target_include_directories(Tools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ToolsTest test.cpp)
target_link_libraries(ToolsTest PUBLIC Tools)