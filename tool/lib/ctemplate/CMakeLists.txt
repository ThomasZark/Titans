cmake_minimum_required(VERSION 2.8)

#获取头文件路径
SET (
    DEPEND_INCLUDE_LIST
    ${DEPEND_INCLUDE_LIST}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PARENT_SCOPE
)

#获取链接路径
SET (
    DEPEND_LINK_LIST
    ${DEPEND_LINK_LIST}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    PARENT_SCOPE
)

#获取链接库
SET (
    DEPEND_LIB_LIST
    ${DEPEND_LIB_LIST}
    libctemplate.a
    PARENT_SCOPE
)