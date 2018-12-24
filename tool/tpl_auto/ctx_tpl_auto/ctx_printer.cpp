#include "ctx_printer.h"
#include "common.h"
#include "util.h"

#include <iostream>
#include <fstream>

using namespace ctemplate;
using namespace google::protobuf;
using namespace google::protobuf::compiler;
using namespace std;

void CreateSevice(const ServiceDescriptor* pServiceDesc, stEnv_t* env)
{
    std::string tpl_path = env->arg.tpl_path;
    TemplateDictionary& dict = *(env->dict.dict1);

    Print(tpl_path + "src/common/common.h", dict, env->conf.src_common_path + "common.h");
    Print(tpl_path + "src/common/attr_define.h", dict, env->conf.src_common_path + "attr_define.h");
    Print(tpl_path + "src/msg/BigMsg.h", dict, env->conf.src_msg_path + env->conf.CAMEL_SRV_NAME + "Msg.h");

    Print(tpl_path + "src/plugin/Setting.cpp", dict,  env->conf.src_plugin_path + "Setting.cpp");
    Print(tpl_path + "src/plugin/Setting.h", dict, env->conf.src_plugin_path + "Setting.h");

    Print(tpl_path + "src/CMakeLists.txt", dict, env->conf.src_path + "CMakeLists.txt");
    Print(tpl_path + "src/SvrInit.cpp", dict, env->conf.src_path + "SvrInit.cpp");
    
    Print(tpl_path + "conf/RpcMgr.conf", dict, env->conf.conf_path + "RpcMgr.conf");
    Print(tpl_path + "conf/TmplMgr.conf", dict, env->conf.conf_path + env->conf.CAMEL_SRV_NAME + "Mgr.conf");
    Print(tpl_path + "conf/Tmpl.conf", dict, env->conf.conf_path + env->conf.CAMEL_SRV_NAME + ".conf");

    for (int i = 0; i < pServiceDesc->method_count(); i++) {
        const MethodDescriptor* pMethodDesc = pServiceDesc->method(i);
        auto iter = env->dict.method_dict.find(pMethodDesc->name());
        if(iter != env->dict.method_dict.end()) {
            Print(tpl_path + "src/msg/SubMsg.cpp", *(iter->second), env->conf.src_msg_path + pMethodDesc->name() + "Msg.cpp");
        }
    }

    char cmd[500] = {0};
    string protoc_file = env->arg.proto_dir + env->arg.proto_file;
    cout << GREEN << "[正在拷贝 proto]" << RESET << endl;
    snprintf(cmd, sizeof(cmd), "cp -r %s %s", protoc_file.c_str(), env->conf.proto_path.c_str());
    system(cmd);
}

int CreateAllDir(stEnv_t* env)
{
    env->conf.src_path = env->arg.target_dir + env->conf.SRV_NAME + "/src/";
    env->conf.src_common_path = env->arg.target_dir + env->conf.SRV_NAME + "/src/common/";
    env->conf.conf_path = env->arg.target_dir + env->conf.SRV_NAME + "/conf/";
    env->conf.src_msg_path = env->arg.target_dir + env->conf.SRV_NAME + "/src/msg/";
    env->conf.src_plugin_path = env->arg.target_dir + env->conf.SRV_NAME + "/src/plugin/";
    env->conf.build_path = env->arg.target_dir + env->conf.SRV_NAME + "/build/";
    env->conf.proto_path = env->arg.target_dir + env->conf.SRV_NAME + "/proto/";

    cout << GREEN << "[创建代码src目录]" << RESET << endl;
    if (!CreateDir(env->conf.src_path.c_str())) {
        cout << "create dir failed, src_path:" << env->conf.src_path << endl;
        return -1;
    }

    cout << GREEN << "[创建代码build目录]" << RESET << endl;
    if (!CreateDir(env->conf.build_path.c_str())) {
        cout << "create dir failed, build_path:" << env->conf.build_path << endl;
        return -1;
    }

    cout << GREEN << "[创建代码conf目录]" << RESET << endl;
    if (!CreateDir(env->conf.conf_path.c_str())) {
        cout << "create  dir failed, conf_path:" << env->conf.conf_path << endl;
        return -1;
    }

    cout << GREEN << "[创建代码proto目录]" << RESET << endl;
    if (!CreateDir(env->conf.proto_path.c_str())) {
        cout << "create dir failed, proto_path:" << env->conf.proto_path << endl;
        return -1;
    }

    cout << GREEN << "[创建代码src/common目录]" << RESET << endl;
    if (!CreateDir(env->conf.src_common_path.c_str())) {
        cout << "create dir failed, src_common_path:" << env->conf.src_common_path << endl;
        return -1;
    }

    cout << GREEN << "[创建代码src/msg目录]" << RESET << endl;
    if (!CreateDir(env->conf.src_msg_path.c_str())) {
        cout << "create dir failed, src_msg_path:" << env->conf.src_msg_path << endl;
        return -1;
    }

    cout << GREEN << "[创建代码msg/plugin目录]" << RESET << endl;
    if (!CreateDir(env->conf.src_plugin_path.c_str())) {
        cout << "create dir failed, src_plugin_path:" << env->conf.src_plugin_path << endl;
        return -1;
    }

    cout << GREEN <<"[创建服务目录成功]" << RESET << endl;
    return 0;
}

int CreateProject(stEnv_t* env) {

    if(CreateAllDir(env) != 0) {
        return -1;
    }

    for (int index = 0; index < env->desc.pFileDescriptor->service_count(); index++) {
        const ServiceDescriptor* pServiceDesc = env->desc.pFileDescriptor->service(index);
        CreateSevice(pServiceDesc, env);
    }
    return 0;
}

int CreateMethod(stEnv_t* env) {

    if(CreateAllDir(env) != 0) {
        return -1;
    }
    
    char cmd[500] = {0};
    snprintf(cmd, sizeof(cmd), "chmod 755 -R %s", env->conf.src_path.c_str());
    system(cmd);

    for (int index = 0; index < env->desc.pFileDescriptor->service_count(); index++) {
        const ServiceDescriptor* pServiceDesc = env->desc.pFileDescriptor->service(index);

        for (int i = 0; i < pServiceDesc->method_count(); i++) {
            const MethodDescriptor* pMethodDesc = pServiceDesc->method(i);
            if(pMethodDesc->name() == env->arg.method) {
                auto iter = env->dict.method_dict.find(pMethodDesc->name());
                if(iter != env->dict.method_dict.end()) {
                    Print(env->arg.tpl_path + "msg/SubMsg.cpp", *(iter->second), 
                        env->conf.src_msg_path + pMethodDesc->name() + "Msg.cpp");
                }
                return 0;
            }
        }
    }
    cout << RED <<"没有找到对应的RPC方法" << RESET <<env->arg.method << endl;
    return -1;
}
