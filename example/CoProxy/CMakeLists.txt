cmake_minimum_required(VERSION 2.8)
project(CoProxySvr)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

find_package(Protobuf REQUIRED)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/server/CoProxySvr.proto)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/server/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/client/)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROTOBUF_INCLUDE_DIRS})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/server/ SERVER)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/client/ CLIENT)

add_executable(co_proxy_svr ${SERVER} ${PROTO_HDRS} ${PROTO_SRCS})
add_executable(co_proxy_cli ${CLIENT} ${PROTO_HDRS} ${PROTO_SRCS})

target_link_libraries(co_proxy_svr ${PROTOBUF_LIBRARIES} libcolib.a dl)
target_link_libraries(co_proxy_cli ${PROTOBUF_LIBRARIES})