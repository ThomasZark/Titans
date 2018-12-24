project(titans_tool)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

aux_source_directory(. DIR_SRCS)
add_executable(attr_incr ${DIR_SRCS})
target_link_libraries(attr_incr pthread gflags)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/attr_incr DESTINATION bin)