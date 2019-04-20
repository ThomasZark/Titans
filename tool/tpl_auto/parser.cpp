#include "parser.h"
#include "util.h"
#include "common.h"

#include <iostream>

using namespace ctemplate;
using namespace google::protobuf;
using namespace google::protobuf::compiler;
using namespace std;

void InitService(const ServiceDescriptor* pServiceDesc, stEnv_t* env) {

    env->conf.SRV_NAME = pServiceDesc->name();
    env->conf.UP_SRV_NAME = env->conf.SRV_NAME;
    UperString(env->conf.UP_SRV_NAME);
    env->conf.CAMEL_SRV_NAME = GetClassName(env->conf.SRV_NAME);
    env->conf.PACKAGE = env->desc.pFileDescriptor->package();
    ConvertPackage(env->conf.PACKAGE);
    env->conf.PROTO = env->desc.pFileDescriptor->name().substr(0, env->desc.pFileDescriptor->name().size() - 6);

    ctemplate::TemplateDictionary* dict = env->dict.dict1;
    cout << GREEN << "[服务名称]" << RESET << env->conf.SRV_NAME << RESET << endl;
    dict->SetValue("SRV_NAME", env->conf.SRV_NAME);
    dict->SetValue("UP_SRV_NAME", env->conf.UP_SRV_NAME);
    dict->SetValue("CAMEL_SRV_NAME", env->conf.CAMEL_SRV_NAME);
    dict->SetValue("PACKAGE", env->conf.PACKAGE);
    dict->SetValue("HEAD_PREFIX", env->arg.head_prefix);
    dict->SetValue("PROTO", env->conf.PROTO);
    dict->SetValue("USER", env->para.USE_USER);
    dict->SetValue("SERVICE_PORT", ToStr(env->para.USE_PORT));
    dict->SetValue("SERVICE_IPS",env->para.USE_IPS);
}

int CheckService(const ServiceDescriptor* pServiceDesc, stEnv_t* env) {

    if(env->arg.strict == 1) {
        cout << GREEN << "不做检查 proto:"<< env->conf.PROTO;
    }

    if(0 != strcmpIgnoreCase(env->conf.PROTO.c_str(), env->conf.SRV_NAME.c_str()))
    {
        if(env->arg.strict == 0) {
            cout << RED << "服务名与proto文件名不对应，" <<RESET << "proto:" << env->conf.PROTO << "，svr_name:" << env->conf.SRV_NAME << endl;
            return -1;
        }
    }

    const EnumDescriptor* pBigCmdEnumDescriptor = nullptr;
    pBigCmdEnumDescriptor = env->desc.pFileDescriptor->FindEnumTypeByName("BIG_CMD");
    if (nullptr == pBigCmdEnumDescriptor)
    {
        cout << RED <<"[ERROR!!!BIG_CMD is none]"<< RESET << endl;
        return -1;
    }

    google::protobuf::SourceLocation location;
    if (pBigCmdEnumDescriptor->value(0)->GetSourceLocation(&location)) {
        if (0 == location.trailing_comments.size())
        {
            if(env->arg.strict == 0) {
                cout << RED << pBigCmdEnumDescriptor->name() <<  "." <<  pBigCmdEnumDescriptor->value(0)->name() << "没有注释" << RESET << endl;
                return -1;
            }
        }
    }
    env->desc.pBigCmdDesc = pBigCmdEnumDescriptor;

    const EnumDescriptor* pSubCmdEnumDescriptor = nullptr;
    pSubCmdEnumDescriptor = env->desc.pFileDescriptor->FindEnumTypeByName("SUB_CMD");
    if (nullptr == pSubCmdEnumDescriptor)
    {
        cout << RED <<"[ERROR!!!SUB_CMD is none]" << RESET << endl;
        return -1;
    }
    env->desc.pSubCmdDesc = pSubCmdEnumDescriptor;

    if (pServiceDesc->method_count() != pSubCmdEnumDescriptor->value_count())
    {
        cout << RED <<"[ERROR!!!子命令个数和函数个数不对应，子命令个数：" << pSubCmdEnumDescriptor->value_count() << ",函数个数：" << pServiceDesc->method_count() << "]"<< RESET << endl;
        return -1;
    }

    for (int i = 0; i < pServiceDesc->method_count(); i++) {
        const MethodDescriptor* pMethodDesc = pServiceDesc->method(i);
        const EnumValueDescriptor* pEnumDesc = pSubCmdEnumDescriptor->value(i);
        google::protobuf::SourceLocation location;
        if (pEnumDesc->GetSourceLocation(&location)) {
            if (0 == location.trailing_comments.size())
            {
                if(env->arg.strict == 0) {
                    cout << RED << pSubCmdEnumDescriptor->name() <<  "." <<  pEnumDesc->name() << "没有注释" << RESET <<endl;
                    return -1;
                }
            }
        }
        if (0 != strcmpIgnoreCase(pMethodDesc->name().c_str(), pSubCmdEnumDescriptor->value(i)->name().c_str()))
        {
            if(env->arg.strict == 0) {
                cout << RED <<"[ERROR!!!子命令和函数名称不对应，子命令：" << pSubCmdEnumDescriptor->value(i)->name() << ",函数名：" << pMethodDesc->name() << "]" << RESET << endl;
                return -1;
            }
        }
    }
    
    return 0;
}

void readTestMessage(const Descriptor* pDescriptor, string name, const FileDescriptor* pFileDescriptor, string& result)
{
    for (int j = 0; j < pDescriptor->field_count(); j++) {
        string op = "set";
        string var = "tmp";
        const FieldDescriptor* pFieldDescriptor = pDescriptor->field(j);
        if (FieldDescriptor::CPPTYPE_MESSAGE == pFieldDescriptor->cpp_type()) {
            op = "mutable";
            if (pFieldDescriptor->is_repeated()) {
                op = "add";
            }
            var = "p" + pFieldDescriptor->name();
            result += pFieldDescriptor->message_type()->name() + "* " + var + " = " + "p" + name + "->" + op + "_" + pFieldDescriptor->name() + "();\n    ";
            const Descriptor* pstDescriptor = pFieldDescriptor->message_type();
            readTestMessage(pstDescriptor, pFieldDescriptor->name(), pFileDescriptor, result);
            continue;
        }

        if (pFieldDescriptor->is_repeated()) {
            op = "add";
        }
        switch (pFieldDescriptor->cpp_type()) {
            case FieldDescriptor::CPPTYPE_INT32:
            case FieldDescriptor::CPPTYPE_UINT32:
            case FieldDescriptor::CPPTYPE_INT64:
            case FieldDescriptor::CPPTYPE_UINT64: {
                result += "p" + name + "->" + op + "_" + pFieldDescriptor->name() + "(1);\n    ";
                break;
            }
            case FieldDescriptor::CPPTYPE_BOOL: {
                result += "p" + name + "->" + op + "_" + pFieldDescriptor->name() + "(true);\n    ";
                break;
            }
            case FieldDescriptor::CPPTYPE_STRING: {
                result += "p" + name + "->" + op + "_" + pFieldDescriptor->name() + "(\"auto_test\");\n    ";
                break;
            }
            case FieldDescriptor::CPPTYPE_DOUBLE:
            case FieldDescriptor::CPPTYPE_FLOAT: {
                result += "p" + name + "->" + op + "_" + pFieldDescriptor->name() + "(1.1);\n    ";
                break;
            }
            case FieldDescriptor::CPPTYPE_ENUM: {
                const EnumDescriptor* enDescriptor = pFieldDescriptor->enum_type();
                const EnumValueDescriptor* valDescriptor = enDescriptor->value(0);
                result += "p" + name + "->" + op + "_" + pFieldDescriptor->name() + "(" + valDescriptor->name() + ");\n    ";
                break;
            }
            default: {
                continue;
            }
        }
    }
    return;
}

void readTopMessage(const Descriptor* pDescriptor, const FileDescriptor* pFileDescriptor, TemplateDictionary& tmp_dict)
{
    for (int j = 0; j < pDescriptor->field_count(); j++) {
        const FieldDescriptor* pFieldDescriptor = pDescriptor->field(j);
        if (FieldDescriptor::CPPTYPE_MESSAGE == pFieldDescriptor->cpp_type()) {
            TemplateDictionary* tmp_dict1 = tmp_dict.AddSectionDictionary("MESSAGE_ITEM");
            tmp_dict1->SetValue("NAME", pFieldDescriptor->name());
            tmp_dict1->SetValue("OP", "mutable");
            if (pFieldDescriptor->is_repeated()) {
                tmp_dict1->SetValue("OP", "add");
            }
            tmp_dict1->SetValue("MESSAGE_TYPE", pFieldDescriptor->message_type()->name());
            tmp_dict1->SetValue("VAR", "p" + pFieldDescriptor->name());
            const Descriptor* pstDescriptor = pFieldDescriptor->message_type();
            string result;
            readTestMessage(pstDescriptor, pFieldDescriptor->name(), pFileDescriptor, result);
            tmp_dict1->SetValue("CODE", result);
            continue;
        }

        TemplateDictionary* tmp_dict1 = tmp_dict.AddSectionDictionary("ITEM");
        tmp_dict1->SetValue("NAME", pFieldDescriptor->name());
        tmp_dict1->SetValue("OP", "set");
        if (pFieldDescriptor->is_repeated()) {
            tmp_dict1->SetValue("OP", "add");
        }

        switch (pFieldDescriptor->cpp_type()) {
            case FieldDescriptor::CPPTYPE_INT32:
            case FieldDescriptor::CPPTYPE_UINT32:
            case FieldDescriptor::CPPTYPE_INT64:
            case FieldDescriptor::CPPTYPE_UINT64: {
                tmp_dict1->SetValue("VALUE", "1");
                break;
            }
            case FieldDescriptor::CPPTYPE_BOOL: {
                tmp_dict1->SetValue("VALUE", "true");
                break;
            }
            case FieldDescriptor::CPPTYPE_ENUM: {
                const EnumDescriptor* enDescriptor = pFieldDescriptor->enum_type();
                const EnumValueDescriptor* valDescriptor = enDescriptor->value(0);
                tmp_dict1->SetValue("VALUE", valDescriptor->name());
                break;
            }
            case FieldDescriptor::CPPTYPE_STRING: {
                tmp_dict1->SetValue("VALUE", "\"auto_test\"");
                break;
            }
            case FieldDescriptor::CPPTYPE_DOUBLE:
            case FieldDescriptor::CPPTYPE_FLOAT: {
                tmp_dict1->SetValue("VALUE", "1.1");
                break;
            }
            default: {
                continue;
            }
        }
    }
    return;
}

void ParseTestCode(const MethodDescriptor* pMethodDesc, stEnv_t* env)
{
    bool flag = false;
    const Descriptor* pReqDescriptor = env->desc.pFileDescriptor->FindMessageTypeByName(pMethodDesc->input_type()->name());
    if (nullptr == pReqDescriptor) {
        cout << RED<< "Can not find message type" << pMethodDesc->input_type()->full_name() << RESET << endl; 
        const DescriptorPool *pool = env->desc.pFileDescriptor->pool();
        pReqDescriptor = pool->FindMessageTypeByName(pMethodDesc->input_type()->full_name());
        if (nullptr == pReqDescriptor)
        {
            cout << RED<< "Can not find message type" << pMethodDesc->input_type()->full_name() << RESET << endl;  
            return;
        }
        flag = true;
    }

    string cmd_name = pMethodDesc->name();
    TemplateDictionary* dict1 = env->dict.test_dict->AddSectionDictionary("CMD_TEST");
    if (flag)
    {
        string full_name = pMethodDesc->input_type()->full_name();
        ConvertPackage(full_name);
        dict1->SetValue("REQ", full_name);
    }
    else
    {
        dict1->SetValue("REQ", pMethodDesc->input_type()->name());
    }

    flag = false;
    const Descriptor* pRspDescriptor = env->desc.pFileDescriptor->FindMessageTypeByName(pMethodDesc->output_type()->name());
    if (nullptr == pRspDescriptor)
    {
        cout << RED << "Can not find message type" << pMethodDesc->output_type()->full_name() << RESET << endl; 
        const DescriptorPool *pool = env->desc.pFileDescriptor->pool();
        pRspDescriptor = pool->FindMessageTypeByName(pMethodDesc->output_type()->full_name());
        if (nullptr == pReqDescriptor)
        {
            cout << RED << "Can not find message type" << pMethodDesc->output_type()->full_name() << RESET << endl; 
            return;
        }
        flag = true;
    }
    if (flag)
    {
        string full_name = pMethodDesc->output_type()->full_name();
        ConvertPackage(full_name);
        dict1->SetValue("RSP", full_name);
    }
    else
    {
        dict1->SetValue("RSP", pMethodDesc->output_type()->name());
    }
    dict1->SetValue("FUNC_NAME", pMethodDesc->name());
    readTopMessage(pReqDescriptor, env->desc.pFileDescriptor, *dict1);
}

void ParseMethodInner(const MethodDescriptor* pMethodDesc, TemplateDictionary* dict1, stEnv_t* env) {

    dict1->SetValue("FUNC_NAME", pMethodDesc->name());
    dict1->SetValue("MONITOR", "MONITOR_" + pMethodDesc->name());

    bool flag = false;
    const Descriptor* pReqDescriptor = env->desc.pFileDescriptor->FindMessageTypeByName(pMethodDesc->input_type()->name());
    if (nullptr == pReqDescriptor) {
        const DescriptorPool *pool = env->desc.pFileDescriptor->pool();
        pReqDescriptor = pool->FindMessageTypeByName(pMethodDesc->input_type()->full_name());
        if (nullptr == pReqDescriptor)
        {
            cout << RED<< "Can not find message type" << pMethodDesc->input_type()->full_name() << RESET << endl; 
            return;
        }
        flag = true;
    }

    if (flag)
    {
        string full_name = pMethodDesc->input_type()->full_name();
        ConvertPackage(full_name);
        dict1->SetValue("REQ_TYPE", full_name);
    }
    else
    {
        dict1->SetValue("REQ_TYPE", pMethodDesc->input_type()->name());
    }

    readTopMessage(pReqDescriptor, env->desc.pFileDescriptor, *dict1);

    flag = false;
    const Descriptor* pRspDescriptor = env->desc.pFileDescriptor->FindMessageTypeByName(pMethodDesc->output_type()->name());
    if (nullptr == pRspDescriptor) {
        const DescriptorPool *pool = env->desc.pFileDescriptor->pool();
        pRspDescriptor = pool->FindMessageTypeByName(pMethodDesc->output_type()->full_name());
        if (nullptr == pRspDescriptor)
        {
            cout << RED << "Can not find message type" << pMethodDesc->output_type()->full_name() << RESET << endl; 
            return;
        }
        flag = true;
    }

    if (flag)
    {
        string full_name = pMethodDesc->output_type()->full_name();
        ConvertPackage(full_name);
        dict1->SetValue("RSP_TYPE", full_name);
    }
    else
    {
        dict1->SetValue("RSP_TYPE", pMethodDesc->output_type()->name());
    }
}

int ParseMethod(const MethodDescriptor* pMethodDesc, const EnumValueDescriptor* pEnumDesc, stEnv_t* env) {

    TemplateDictionary* dict = env->dict.dict1;
    TemplateDictionary* dict1 = env->dict.dict1->AddSectionDictionary("PROC_FUNC");
    TemplateDictionary* method = new TemplateDictionary(pEnumDesc->name());
    TemplateDictionary* method1 = method->AddSectionDictionary("PROC_FUNC");

    //proc common env
    dict1->SetValue("BIG_CMD", env->desc.pBigCmdDesc->value(0)->name());
    dict->SetValue("BIG_CMD", env->desc.pBigCmdDesc->value(0)->name());
    dict1->SetValue("SUB_CMD", pEnumDesc->name());

    method1->SetValue("BIG_CMD", env->desc.pBigCmdDesc->value(0)->name());
    method1->SetValue("SUB_CMD", pEnumDesc->name());
    method1->SetValue("SRV_NAME", env->conf.SRV_NAME);
    method1->SetValue("PROTO", env->conf.PROTO);
    method1->SetValue("PACKAGE", env->conf.PACKAGE);
    method1->SetValue("CAMEL_SRV_NAME", env->conf.CAMEL_SRV_NAME);

    //proc sub env
    ParseMethodInner(pMethodDesc, dict1, env);
    ParseMethodInner(pMethodDesc, method1, env);

    ParseTestCode(pMethodDesc, env);

    env->dict.method_dict[pMethodDesc->name()] = method1;
    return 0;
}

void ParseServiceDict(const ServiceDescriptor* pServiceDesc, stEnv_t* env) {

    TemplateDictionary* dict = env->dict.dict1;

    if (env->para.ADD_METHOD == true) {
        cout << GREEN <<"[增加method]" << RESET << env->arg.method << endl;
    }

    int dependency_cnt = env->desc.pFileDescriptor->dependency_count();
    for (int i = 0; i < dependency_cnt; i++)
    {
        const FileDescriptor* dep_fd = env->desc.pFileDescriptor->dependency(i);
        TemplateDictionary* dict1 = dict->AddSectionDictionary("IMPORT_PROTO");
        dict1->SetValue("IMPORT_PB", dep_fd->name().substr(0, dep_fd->name().size() - 5));
    }
}

int ParseService(const ServiceDescriptor* pServiceDesc, stEnv_t* env) {

    InitService(pServiceDesc, env);
    if(CheckService(pServiceDesc, env) != 0) {
        return -1;
    }
    ParseServiceDict(pServiceDesc, env);

    for (int i = 0; i < pServiceDesc->method_count(); i++) {
        const EnumValueDescriptor* pEnumDesc = env->desc.pSubCmdDesc->value(i);
        if (0 != ParseMethod(pServiceDesc->method(i), pEnumDesc, env))
        {
            return -1;
        }
    }
    return 0;
}

int ParseFile(stEnv_t* env) {

    for (int i = 0; i < env->desc.pFileDescriptor->service_count(); i++) {
        if (0 != ParseService(env->desc.pFileDescriptor->service(i), env))
        {
            return -1;
        }
    }
    return 0;
}