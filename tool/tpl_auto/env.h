#ifndef _ENV_H_
#define _ENV_H_

#include <ctemplate/template.h>

#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

#include <string>
#include <map>

struct stArg_t {

    std::string proto_dir;
    std::string target_dir;
    std::string tpl_path;
    std::string test_tpl_path;
    std::string head_prefix;
    std::string method;
    std::string proto_file;
    uint32_t strict;
};

struct stPara_t {

    bool ADD_METHOD;
    bool ADD_PLUGIN;
    int USE_PORT;
    std::string USE_USER;
    std::string USE_IPS;
};

struct stConf_t {

    std::string PACKAGE;
    std::string UP_SRV_NAME;
    std::string SRV_NAME;
    std::string CAMEL_SRV_NAME;
    std::string PROTO;

    std::string src_path;
    std::string src_common_path;
    std::string conf_path;
    std::string src_msg_path;
    std::string src_plugin_path;
    std::string build_path;
    std::string proto_path;
};

struct stDesc_t {

    const google::protobuf::FileDescriptor*  pFileDescriptor;
    const google::protobuf::EnumDescriptor* pBigCmdDesc;
    const google::protobuf::EnumDescriptor* pSubCmdDesc;
};

struct stDict_t {

    ctemplate::TemplateDictionary* dict1;
    ctemplate::TemplateDictionary* test_dict;
    std::map<std::string, ctemplate::TemplateDictionary*> method_dict;
};

struct stEnv_t {

    stArg_t arg;
    stPara_t para;
    stConf_t conf;
    stDesc_t desc;
    stDict_t dict;
};

void InitEnv(int* args, char*** argv, bool, stEnv_t* env);

#define RPC_TEST 0

#endif //_ENV_H_


