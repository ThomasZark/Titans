cmake_minimum_required(VERSION 2.8)
project({{CAMEL_SRV_NAME}})
set(CMAKE_VERBOSE_MAKEFILE off)

#find需要的cmake package
find_package(spp REQUIRED)

#设置编译选项
add_definitions("-g -fPIC -shared -Wall -pipe -fno-ident -MMD -Werror -D_GNU_SOURCE -D_REENTRANT -std=c++11")
add_definitions("-Wno-builtin-macro-redefined -D__FILE__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

#环境变量
set(TITANS $ENV{TITANS_HOME})

function(PROTO_RPC_GENERATE PROTO_NAME)
   execute_process(COMMAND ctx_rpc_auto -proto_dir=${CMAKE_BINARY_DIR}/proto 
                                        -proto_file=${PROTO_NAME}
                                        -target_path=${CMAKE_BINARY_DIR}/rpc 
                                        -tpl_path=${TITANS}/tool/template/ctx_tpl_v3)
endfunction()
function(RICK_PROTO_ATTACH SERVER)
   execute_process(COMMAND ilive_rick_tool -proto_dir=${CMAKE_BINARY_DIR}/proto -svr_name=${SERVER})
endfunction()

#拷贝依赖的proto
execute_process(COMMAND mkdir -p ${CMAKE_BINARY_DIR}/proto)
execute_process(COMMAND mkdir -p ${CMAKE_BINARY_DIR}/rpc)
RICK_PROTO_ATTACH("{{PROTO}}")

#编译生成proto
file(GLOB_RECURSE PROTO_FILE ${CMAKE_BINARY_DIR}/proto/*.proto)
execute_process(COMMAND protoc --cpp_out=${CMAKE_BINARY_DIR}/proto
--proto_path=${CMAKE_BINARY_DIR}/proto ${PROTO_FILE})

#生成依赖rpc文件
PROTO_RPC_GENERATE({{PROTO}}.proto)

#获取依赖模块
set(DEPEND_INCLUDE_LIST ${TITANS})
set(DEPEND_SOURCE_LIST ${TITANS})
set(DEPEND_LIB_LIST "")
set(DEPEND_LINK_LIST ${TITANS})

ADD_SUBDIRECTORY(${TITANS}/spp/server/ ${CMAKE_BINARY_DIR}/spp/server/)
ADD_SUBDIRECTORY(${TITANS}/spp/handler/ctx/ilive_ctx ${CMAKE_BINARY_DIR}/spp/handler/ctx/ilive_ctx)
ADD_SUBDIRECTORY(${TITANS}/spp/plugin/mt_plugin/ ${CMAKE_BINARY_DIR}/spp/plugin/mt_plugin)
ADD_SUBDIRECTORY(${TITANS}/spp/plugin/rpc_plugin ${CMAKE_BINARY_DIR}/spp/plugin/rpc_plugin)
ADD_SUBDIRECTORY(${TITANS}/spp/plugin/uls_plugin ${CMAKE_BINARY_DIR}/spp/plugin/uls_plugin)
ADD_SUBDIRECTORY(${TITANS}/spp/plugin/conf_plugin ${CMAKE_BINARY_DIR}/spp/plugin/conf_plugin)
{{#TDBANK}}ADD_SUBDIRECTORY(${TITANS}/spp/plugin/tc_plugin ${CMAKE_BINARY_DIR}/spp/plugin/tc_plugin){{/TDBANK}}
{{#FLOW_LOG}}ADD_SUBDIRECTORY(${TITANS}/spp/plugin/flow_plugin ${CMAKE_BINARY_DIR}/spp/plugin/flow_plugin){{/FLOW_LOG}}
{{#TMEM}}ADD_SUBDIRECTORY(${TITANS}/spp/plugin/tmem_plugin ${CMAKE_BINARY_DIR}/spp/plugin/tmem_plugin){{/TMEM}} 
{{#HIPPO}}ADD_SUBDIRECTORY(${TITANS}/spp/plugin/hippo_consumer_plugin/ ${CMAKE_BINARY_DIR}/spp/plugin/hippo_consumer_plugin/){{/HIPPO}}
{{#HIPPO_SENDER}}ADD_SUBDIRECTORY(${TITANS}/spp/plugin/hippo_producer_plugin/ ${CMAKE_BINARY_DIR}/spp/plugin/hippo_producer_plugin/){{/HIPPO_SENDER}}

#设定include目录
include_directories(${DEPEND_INCLUDE_LIST})
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/proto)
include_directories(${CMAKE_BINARY_DIR}/rpc)

#获取目录下的c/cpp文件合集
aux_source_directory(${CMAKE_SOURCE_DIR} DIR_SRCS)
aux_source_directory(${CMAKE_SOURCE_DIR}/msg DIR_SRCS)
aux_source_directory(${CMAKE_SOURCE_DIR}/plugin DIR_SRCS)
aux_source_directory(${CMAKE_BINARY_DIR}/proto DIR_SRCS)
aux_source_directory(${CMAKE_BINARY_DIR}/rpc DIR_SRCS)

#编译链接目录
link_directories(${DEPEND_LINK_LIST})

#编译成动态库
add_library({{CAMEL_SRV_NAME}} SHARED ${DIR_SRCS} ${DEPEND_SOURCE_LIST}) 

#添加链接库
target_link_libraries({{CAMEL_SRV_NAME}} 
                        attrapi 
                        qos_client64 
                        config++ 
                        sproto 
                        sppcommon
                        ${DEPEND_LIB_LIST})