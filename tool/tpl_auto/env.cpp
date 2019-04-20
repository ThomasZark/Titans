#include "env.h"
#include "common.h"

#include <gflags/gflags.h>
#include <json/json.h>

using namespace std;
using namespace ctemplate;
using namespace google::protobuf;
using namespace google::protobuf::compiler;

DEFINE_string(proto_dir, "", "proto文件所在目录的绝对路径");
DEFINE_string(target_path, "", "代码生成目标目录的绝对路径");
DEFINE_string(tpl_path, "","模板文件目录路径");
DEFINE_string(test_tpl_path, "/","测试代码模板目录");
DEFINE_string(proto_file, "", "proto文件名(无需目录前缀)");
DEFINE_string(param, "{\"add_method\":1, \"flow_log\":1}", "json配置服务所需特性");
DEFINE_string(head_prefix, "", "头文件define前缀");
DEFINE_string(method, "", "增加的子命令字");
DEFINE_int32(strict, 0, "0 严格检查, 1不检查");

void FormatPath(stEnv_t* env) {

    std::string proto_dir = FLAGS_proto_dir;
    if (proto_dir.size() == 0)
    {
        char line[1024];
        FILE* file=popen("pwd", "r");
        if (file != nullptr)
        {
            if (fgets(line, 1024, file) != nullptr)
            {
                proto_dir = line;
                proto_dir[proto_dir.length() - 1] = '/';
            }
        }
    }

    if (proto_dir[proto_dir.length() - 1] != '/') 
    {
        proto_dir += "/";
    }
    env->arg.proto_dir = proto_dir;

    std::string target_path = FLAGS_target_path;
    if (target_path.size() == 0)
    {
        char line[1024];
        FILE* file=popen("pwd", "r");
        if (file != nullptr)
        {
            if (fgets(line, 1024, file) != nullptr)
            {
                target_path = line;
                target_path[target_path.length() - 1] = '/';
            }
        }
    }

    if (target_path[target_path.length() - 1] != '/') 
    {
        target_path += "/";
    }
    env->arg.target_dir = target_path;

    std::string tpl_path = FLAGS_tpl_path;
    if (tpl_path[tpl_path.length() - 1] != '/') {
        tpl_path += "/";
    }
    env->arg.tpl_path = tpl_path;

    std::string test_tpl_path = FLAGS_test_tpl_path;
    if (test_tpl_path[test_tpl_path.length() - 1] != '/') {
        test_tpl_path += "/";
    }
    env->arg.test_tpl_path = test_tpl_path;
    return; 
}

void ProcPara(stEnv_t* env)
{
    env->para.ADD_METHOD = false; 
    env->para.ADD_PLUGIN = false; 
    env->para.USE_FLOW = false; 
    env->para.USE_USER = "unknown";
    env->para.USE_PORT = 0;


    Json::Value value;
    try {
        value.decodeJson(FLAGS_param);
    } catch (std::exception& e) {
        cout << "decode json failed,err=" << e.what() << endl;
    }

    if(value.hasKey("add_method")) {
        env->para.ADD_METHOD = true;
    }

    if(value.hasKey("add_plugin")) {
        env->para.ADD_PLUGIN = true;
    }

    if (value.hasKey("port")) {
        env->para.USE_PORT = (value["port"].asUInt());
    }

	if (value.hasKey("ip")){
        env->para.USE_IPS = value["ip"].asString();
	}
	

    env->para.USE_USER = value["user"].asString();
    return;
}

void InitEnv(int* args, char*** argv, bool, stEnv_t* env) {

    google::ParseCommandLineFlags(args, argv, true);
    
    FormatPath(env);

    env->arg.head_prefix = FLAGS_head_prefix;
    env->arg.method = FLAGS_method;
    env->arg.proto_file = FLAGS_proto_file;
    env->arg.strict = FLAGS_strict;

    ProcPara(env);
}