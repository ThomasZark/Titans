project(titans_test)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

include_directories(${TITANS})

add_executable(TestProtoCodec TestProtoCodec.cpp)

target_link_libraries(TestProtoCodec 
                            protobuf 
                            sproto 
                            sppcommon 
                            Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(TestProtoCodec)