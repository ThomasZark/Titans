project(titans_tool)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${TITANS_TOOL}/lib/ctemplate/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} DIR_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../ DIR_SRCS)

add_executable(ctx_rpc_auto ${DIR_SRCS})

target_link_libraries(ctx_rpc_auto jsoncpp protobuf pthread glog gflags
                        ${TITANS_TOOL}/lib/ctemplate/lib/libctemplate.a)
