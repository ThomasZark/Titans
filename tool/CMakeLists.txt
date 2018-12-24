cmake_minimum_required(VERSION 2.8)
project(titans_tool)
set(CMAKE_VERBOSE_MAKEFILE off)

find_package(spp REQUIRED)

#环境变量
set(TITANS_TOOL ${CMAKE_CURRENT_SOURCE_DIR})

ADD_SUBDIRECTORY(${TITANS_TOOL}/attr_incr ${CMAKE_BINARY_DIR}/attr_incr)
ADD_SUBDIRECTORY(${TITANS_TOOL}/rpc_incr ${CMAKE_BINARY_DIR}/rpc_incr)
ADD_SUBDIRECTORY(${TITANS_TOOL}/setting_scan ${CMAKE_BINARY_DIR}/setting_scan)
ADD_SUBDIRECTORY(${TITANS_TOOL}/tpl_auto/ctx_tpl_auto ${CMAKE_BINARY_DIR}/tpl_auto/ctx_tpl_auto)
ADD_SUBDIRECTORY(${TITANS_TOOL}/tpl_auto/ctx_rpc_auto ${CMAKE_BINARY_DIR}/tpl_auto/ctx_rpc_auto)