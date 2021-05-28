
add_library(Tools String.cpp Time.cpp FileIO.cpp HashMap.cpp List.cpp)

add_executable(ToolsTest test.cpp)
target_link_libraries(ToolsTest PUBLIC Tools)