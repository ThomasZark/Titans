#include "printer.h"
#include "common.h"
#include "util.h"

#include <iostream>
#include <fstream>

using namespace ctemplate;
using namespace google::protobuf;
using namespace google::protobuf::compiler;
using namespace std;

int CreateRpc(stEnv_t* env) {

    Print(env->arg.tpl_path + "rpc/BaseRpc.cpp", *(env->dict.dict1), env->arg.target_dir + env->conf.CAMEL_SRV_NAME + "Rpc.cpp");
    return 0;
}