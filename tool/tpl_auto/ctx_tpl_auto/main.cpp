#include <iostream>
#include <map>

#include "env.h"
#include "parser.h"
#include "ctx_printer.h"
#include "common.h"
#include "util.h"

using namespace std;
using namespace ctemplate;
using namespace google::protobuf;
using namespace google::protobuf::compiler;

int main(int args, char** argv)
{
    stEnv_t env;
    InitEnv(&args, &argv, true, &env);

    DiskSourceTree sourceTree;
    sourceTree.MapPath("", env.arg.proto_dir);
    Importer importer(&sourceTree, nullptr);

    cout << GREEN << "[正在自动化构建代码]" << RESET << endl;
    cout << GREEN << "[proto文件名]" << RESET << env.arg.proto_file << endl;
    cout << GREEN << "[proto文件目录]" << RESET << env.arg.proto_dir << endl;
    
    env.desc.pFileDescriptor = importer.Import(env.arg.proto_file);
    if (nullptr == env.desc.pFileDescriptor) {
        cout << RED << "无法打开proto文件或者proto文件解释错误：请检查proto文件是否存在或者通过protoc调用检查proto文件是否正确!!!\n" << RESET<< endl;
        return -1;
    }   

    TemplateDictionary dict("example");
    TemplateDictionary test_dict("test_example");

    env.dict.dict1 = &dict;
    env.dict.test_dict = &test_dict;

    if(ParseFile(&env) != 0 ) {
        return -1;
    }

    if(env.para.ADD_METHOD) {
        return CreateMethod(&env);
    }
    else {
        if(CreateProject(&env) != 0) {
            return -1;
        }
    }

    cout << GREEN << "[构建完成]" << RESET << endl;
    return 0;
}
