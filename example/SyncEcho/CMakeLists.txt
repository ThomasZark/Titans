cmake_minimum_required(VERSION 2.8)
project(SyncEcho)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/server/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/client/)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/server/ SERVER)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/client/ CLIENT)

add_executable(echo_server ${SERVER})
add_executable(echo_client ${CLIENT})