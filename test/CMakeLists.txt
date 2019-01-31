cmake_minimum_required(VERSION 2.8)
project(titans_test)
set(CMAKE_VERBOSE_MAKEFILE off)

#find需要的cmake package
find_package(spp REQUIRED)

#环境变量
set(TITANS $ENV{TITANS_HOME})
set(TITANS ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(TITANS_TEST ${CMAKE_CURRENT_SOURCE_DIR})

set(Catch2_DIR ${TITANS_TEST}/catch2/lib64/cmake/Catch2)
find_package(Catch2 REQUIRED)
enable_testing()

include_directories(${TITANS})
include_directories(${CATCH_INCLUDE_DIRS})

#编译单元测试
ADD_SUBDIRECTORY(${TITANS_TEST}/codec/proto ${CMAKE_BINARY_DIR}/codec/proto)