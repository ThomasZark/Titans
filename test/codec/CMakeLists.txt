cmake_minimum_required(VERSION 2.8)
project(titans_test)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

find_package(Protobuf REQUIRED)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS Echo.proto)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROTOBUF_INCLUDE_DIRS})

add_executable(TestCodec
                TestCodec.cpp 
                ${PROTO_HDRS} 
                ${PROTO_SRCS})

target_link_libraries(TestCodec 
                            ${PROTOBUF_LIBRARIES}
                            Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(TestCodec)