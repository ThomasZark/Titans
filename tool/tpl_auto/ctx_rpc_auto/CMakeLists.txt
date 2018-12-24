project(titans_tool)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

find_package(ctemplate REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(protobuf REQUIRED)
find_package(glog REQUIRED)
find_package(gflags REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} DIR_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../ DIR_SRCS)

add_executable(ctx_rpc_auto ${DIR_SRCS})

target_link_libraries(ctx_rpc_auto jsoncpp protobuf pthread glog gflags ctemplate)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ctx_rpc_auto DESTINATION bin)