#
add_library(BPlusTree BPlusTree.hpp)
target_include_directories(BPlusTree INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(BPlusTree PUBLIC Tools)

#add_executable(BPlusTreeTest test.cpp)
#target_link_libraries(BPlusTreeTest PUBLIC Tools)
