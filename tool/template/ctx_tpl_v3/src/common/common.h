#ifndef {{UP_SRV_NAME}}_SRC_COMMON_COMMON_H
#define {{UP_SRV_NAME}}_SRC_COMMON_COMMON_H

enum COMMAND_ID //自动生成
{   
{{#ENUM_CMD}}   {{CMD_ID}}
{{/ENUM_CMD}}
};

//定义业务错误码(0成功,1~10000为系统错误 10001以上为业务错误码)
enum EC_{{UP_SRV_NAME}}_CODE
{
    EC_SUCC = 0,
    EC_RPC_ERROR = -70001,          //RPC网络失败，超时
    EC_RETURN_EMPTY = -70002,       //返回值为空
    EC_PERMIT_ERROR = -70003,       //权限失败
    EC_BACKEND_ERROR = -70004,      //后端返回失败
    EC_LOGIC_ERROR = -70005,        //逻辑业务失败
    EC_PARAM_ERROR = -70006,        //请求参数错误
};

#endif //{{UP_SRV_NAME}}_SRC_COMMON_COMMON_H