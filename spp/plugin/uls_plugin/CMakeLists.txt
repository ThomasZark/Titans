cmake_minimum_required(VERSION 2.8)

#获取源文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ DIR_SRCS)

SET (
    DEPEND_SOURCE_LIST
    ${DEPEND_SOURCE_LIST}
    ${DIR_SRCS}
    PARENT_SCOPE
)

#获取头文件路径

#获取链接路径

#获取链接库
SET (
    DEPEND_LIB_LIST
    ${DEPEND_LIB_LIST}
    logsysapi
    PARENT_SCOPE
)