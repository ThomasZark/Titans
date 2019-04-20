cmake_minimum_required(VERSION 2.8)
project(titans_test)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

add_executable(TestRoute 
                TestRoute.cpp)

target_link_libraries(TestRoute  
                        Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(TestRoute)