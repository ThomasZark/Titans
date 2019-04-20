#ifndef _TITANS_CORE_HANDLER_BASEHANDLER_H_
#define _TITANS_CORE_HANDLER_BASEHANDLER_H_

#include <memory>

namespace TITANS {

namespace HANDLER {

struct stBaseHandlerContext {
    void* arg1;
    void* arg2;
    void* arg3;
};

class BaseHandler {

public:
    BaseHandler() {}

    virtual ~BaseHandler() {}

    //@desc - 处理器初始化
    //@param
    //  pContext    - 初始化参数
    //@return
    //  int     - 0 成功
    virtual int Initialize(std::shared_ptr<struct stBaseHandlerContext> pContext) { 
        return 0; 
    }

    //@desc - 处理器结束
    //@param
    //  pContext    - 初始化参数
    //@return
    virtual void Finalize(std::shared_ptr<struct stBaseHandlerContext> pContext) { 
        return; 
    }

    //@desc - 处理器检查函数
    //@param
    //  pContext    - 初始化参数
    //@return
    virtual int HandleInput(std::shared_ptr<struct stBaseHandlerContext> pContext) { 
        return 0;
    }

    //@desc - 处理器处理函数
    //@param
    //  pContext    - 初始化参数
    //@return
    virtual int HandleProcess(std::shared_ptr<struct stBaseHandlerContext> pContext) {
        return 0;
    };
};

} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_CORE_HANDLER_BASEHANDLER_H_