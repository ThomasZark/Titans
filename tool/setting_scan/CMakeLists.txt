project(titans_tool)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/scan DIR_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/common DIR_SRCS)

add_executable(setting_scan ${DIR_SRCS})
target_link_libraries(setting_scan pthread gflags config++)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/setting_scan DESTINATION bin)
