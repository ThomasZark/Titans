#ifndef _TITANS_CORE_SERVER_BASESERVER_H_
#define _TITANS_CORE_SERVER_BASESERVER_H_

#include <string>
#include <memory>

#include <core/handler/BaseHandler.h>

namespace TITANS {

namespace SERVER {

struct stBaseSvrContext {

};

class BaseServer {

public:
    BaseServer() {}

    virtual ~BaseServer() {}

    //@desc - 服务初始化
    //@param
    //  pContext    - 初始化参数
    //@return
    //  int     - 0 成功
    virtual int Initialize(std::shared_ptr<struct stBaseSvrContext> pContext) { 
        return 0; 
    }

    //@desc - 服务结束
    //@param
    //  pContext    - 初始化参数
    //@return
    //  int     - 0 成功
    virtual void Finalize(std::shared_ptr<struct stBaseSvrContext> pContext) { 
        return; 
    }

    //@desc - 开始服务
    //@param
    //  pContext    - 初始化参数
    //@return
    //  int     - 0 成功
    virtual int Serve(std::shared_ptr<struct stBaseSvrContext> pContext) { 
        return 0; 
    }

    //@desc - 设置服务处理器
    //@param
    //  pBaseHandler    - 处理器
    //@return
    //  BaseServer*     - 服务指针
    virtual BaseServer* SetHandler(std::shared_ptr<HANDLER::BaseHandler> pHandler) { 
        _pHandler = pHandler;
        return this;
    }

    //@desc - 获取服务处理器
    //@param
    //@return
    //  BaseHandler*     - 服务处理器指针
    virtual std::shared_ptr<HANDLER::BaseHandler> GetHandler() { 
        return _pHandler; 
    }

    //@desc - 设置服务名称
    //@param
    //  name    - 服务名称
    //@return
    //  BaseServer*     - 服务指针
    virtual BaseServer* SetServerName(const std::string& name) {
        _name = name;
        return this;
    }

    //@desc - 获取服务名称
    //@param
    //@return
    //  std::string     - 服务名称
    virtual std::string GetServerName() {
        return _name;
    }

protected:
    //处理器指针
    std::shared_ptr<HANDLER::BaseHandler> _pHandler;
    //服务名称
    std::string _name;
};

} //namespace SERVER

} //namespace TITANS

#endif //_TITANS_CORE_SERVER_BASESERVER_H_