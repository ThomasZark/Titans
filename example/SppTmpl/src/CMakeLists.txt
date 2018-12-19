cmake_minimum_required(VERSION 2.8)
project(SppTmplSvr)
set(CMAKE_VERBOSE_MAKEFILE off)

#find需要的cmake package
#find_package(spp-ext REQUIRED)
find_package(spp REQUIRED)

#检查编译器
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    add_definitions("-std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    add_definitions("-std=c++0x")
endif()

#设置编译选项
add_definitions("-g -fPIC -shared -Wall -pipe -fno-ident -MMD -Werror -D_GNU_SOURCE -D_REENTRANT")
add_definitions("-Wno-builtin-macro-redefined -D__FILE__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

#环境变量
set(RPC /data/home/spp_proj/trunk/now_public/)
set(RPC_GEN_TOOL /nfs/svr_template/bin/ctx_rpc_auto)
set(RPC_TPL /nfs/svr_template/template/ctx_tpl_v2)
set(PROTOC protoc)
set(TITANS ${CMAKE_SOURCE_DIR}/../../../)

message(${TITANS})

#设定include目录
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${TITANS})
execute_process(COMMAND mkdir -p ${CMAKE_BINARY_DIR}/proto)
execute_process(COMMAND mkdir -p ${CMAKE_BINARY_DIR}/rpc)
include_directories(${CMAKE_BINARY_DIR}/proto)

#拷贝依赖的proto
execute_process(COMMAND cp ${CMAKE_SOURCE_DIR}/../proto/SppTmplSvr.proto ${CMAKE_BINARY_DIR}/proto)

#编译生成proto
file(GLOB_RECURSE PROTO_FILE ${CMAKE_BINARY_DIR}/proto/*.proto)
execute_process(COMMAND protoc --cpp_out=${CMAKE_BINARY_DIR}/proto
--proto_path=${CMAKE_BINARY_DIR}/proto ${PROTO_FILE})

#生成依赖rpc文件
#execute_process(COMMAND ${RPC_GEN_TOOL} -proto_dir=${CMAKE_BINARY_DIR}/proto -proto_file=TipsChannelSvr.proto -target_path=${CMAKE_BINARY_DIR}/rpc -tpl_path=${RPC_TPL} -strict=1)
#execute_process(COMMAND ${RPC_GEN_TOOL} -proto_dir=${CMAKE_BINARY_DIR}/proto -proto_file=ilive_account_mgr.proto -target_path=${CMAKE_BINARY_DIR}/rpc -tpl_path=${RPC_TPL} -strict=1)

#获取目录下的c/cpp文件合集
aux_source_directory(${CMAKE_SOURCE_DIR} DIR_SRCS)
aux_source_directory(${CMAKE_SOURCE_DIR}/msg DIR_SRCS)
aux_source_directory(${CMAKE_SOURCE_DIR}/plugin DIR_SRCS)
aux_source_directory(${CMAKE_BINARY_DIR}/proto DIR_SRCS)
aux_source_directory(${CMAKE_BINARY_DIR}/rpc DIR_SRCS)

aux_source_directory(${TITANS}/spp/server/ DIR_SRCS)
aux_source_directory(${TITANS}/spp/plugin/mt_plugin DIR_SRCS)
aux_source_directory(${TITANS}/spp/plugin/rpc_plugin DIR_SRCS)
aux_source_directory(${TITANS}/spp/plugin/uls_plugin DIR_SRCS)
aux_source_directory(${TITANS}/spp/plugin/conf_plugin DIR_SRCS)
aux_source_directory(${TITANS}/spp/plugin/tc_plugin DIR_SRCS)
aux_source_directory(${TITANS}/spp/plugin/flow_plugin DIR_SRCS)
aux_source_directory(${TITANS}/spp/plugin/tmem_plugin DIR_SRCS)
aux_source_directory(${TITANS}/spp/plugin/hippo_producer_plugin DIR_SRCS)

#编译成动态库
add_library(SppTmplSvr SHARED ${DIR_SRCS}) 

#添加链接库
target_link_libraries(SppTmplSvr 
                        trmem 
                        tc_api64
                        logsysapi
                        attrapi 
                        qos_client64 
                        config++ 
                        sproto 
                        sppcommon
                        comm_albs_v2_client_api
                        hippo_thread)

# make deploy_test
set(DEPLOY "/nfs/now_ci/deploy")
add_custom_target(deploy_test
    COMMAND cp ../conf/deploy.test.ini ../deploy.ini && cp ../conf/service.test.yaml ./service.yaml && ${DEPLOY}
    DEPENDS ${TipsChannelSvr}
    COMMENT "deploy pkg to test environment..."
)