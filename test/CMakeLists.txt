cmake_minimum_required(VERSION 2.8)
project(titans_test)
set(CMAKE_VERBOSE_MAKEFILE off)

#环境变量
set(TITANS ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(TITANS_TEST ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Catch2 REQUIRED)
enable_testing()

include_directories(${TITANS})
include_directories(${CATCH_INCLUDE_DIRS})

#编译单元测试
ADD_SUBDIRECTORY(${TITANS_TEST}/codec ${CMAKE_BINARY_DIR}/codec)
ADD_SUBDIRECTORY(${TITANS_TEST}/route ${CMAKE_BINARY_DIR}/route)