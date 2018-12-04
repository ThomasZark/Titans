#ifndef _TITANS_CORE_HANDLER_BASEHANDLER_H_
#define _TITANS_CORE_HANDLER_BASEHANDLER_H_

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
    virtual int Initialize(struct stBaseSvrContext* pContext) { 
        return 0; 
    }

    //@desc - 处理器结束
    //@param
    //  pContext    - 初始化参数
    //@return
    virtual void Finalize(struct stBaseSvrContext* pContext) { 
        return; 
    }

    //@desc - 处理器检查函数
    //@param
    //  pContext    - 初始化参数
    //@return
    virtual int HandleInput(struct stBaseHandlerContext* pContext) { 
        return 0;
    }

    //@desc - 处理器处理函数
    //@param
    //  pContext    - 初始化参数
    //@return
    virtual int HandleProcess(struct stBaseHandlerContext* pContext) {};
};

} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_CORE_HANDLER_BASEHANDLER_H_