project(titans_tool)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

#获取依赖模块
set(DEPEND_INCLUDE_LIST ${TITANS_TOOL})
set(DEPEND_SOURCE_LIST ${TITANS_TOOL})
set(DEPEND_LIB_LIST "")
set(DEPEND_LINK_LIST ${TITANS_TOOL})

ADD_SUBDIRECTORY(${TITANS_TOOL}/lib/ctemplate ${CMAKE_BINARY_DIR}/ctx_rpc_auto/lib/ctemplate)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${DEPEND_INCLUDE_LIST})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} DIR_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../ DIR_SRCS)

#编译链接目录
link_directories(${DEPEND_LINK_LIST})

add_executable(ctx_rpc_auto ${DIR_SRCS} ${DEPEND_SOURCE_LIST})

target_link_libraries(ctx_rpc_auto jsoncpp protobuf pthread glog gflags
                        ${DEPEND_LIB_LIST})

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ctx_rpc_auto DESTINATION bin)