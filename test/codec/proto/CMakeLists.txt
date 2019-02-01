cmake_minimum_required(VERSION 2.8)
project(titans_test)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

find_package(Protobuf REQUIRED)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS Echo.proto)

add_executable(TestProtoCodec TestProtoCodec.cpp ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(TestProtoCodec 
                            ${PROTOBUF_LIBRARIES}
                            Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(TestProtoCodec)