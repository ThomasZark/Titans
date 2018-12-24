cmake_minimum_required(VERSION 2.8)
project(titans_tool)
set(CMAKE_VERBOSE_MAKEFILE off)

#环境变量
set(TITANS_TOOL ${CMAKE_CURRENT_SOURCE_DIR})

ADD_SUBDIRECTORY(${TITANS_TOOL}/tpl_auto/ctx_tpl_auto ${CMAKE_BINARY_DIR}/tpl_auto/ctx_tpl_auto)
ADD_SUBDIRECTORY(${TITANS_TOOL}/tpl_auto/ctx_rpc_auto ${CMAKE_BINARY_DIR}/tpl_auto/ctx_rpc_auto)