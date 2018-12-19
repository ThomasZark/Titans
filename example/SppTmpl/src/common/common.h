#ifndef _COMMON_H_
#define _COMMON_H_

//定义业务错误码(0成功,1~10000为系统错误 10001以上为业务错误码)
enum EC_SPP_TMPL_SVR_CODE
{
    EC_SUCC = 0,
    EC_RPC_ERROR = -70001,
    EC_RETURN_EMPTY = -70002,
    EC_INSIDE_ERROR = -70003,
    EC_BACKEND_ERROR = -70004,
    EC_LOGIC_ERROR = -70005,
};

#endif //_COMMON_H_