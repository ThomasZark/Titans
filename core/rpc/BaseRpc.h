#ifndef _TITANS_CORE_RPC_BASERPC_H_
#define _TITANS_CORE_RPC_BASERPC_H_

#include <string>
#include <sstream>
#include <memory>

#include <core/codec/BaseCodec.h>
#include <core/route/BaseRoute.h>
#include <core/net/BaseNet.h>

namespace TITANS {

namespace RPC {

enum {
    RPC_SUCC            = 0,
    RPC_PARAM_ERROR     = -40001,
    RPC_CODEC_ERROR     = -40002,
    RPC_NET_ERROR       = -40003,
    RPC_ROUTE_RERROR    = -40005,
};

class BaseRpc {

public:
    BaseRpc()
    :_pCodec(nullptr)
    ,_pRoute(nullptr)
    ,_pNet(nullptr) {}

    virtual ~BaseRpc() {}

    //@desc - BaseRpc初始化
    //@param
    //  arg1    - 参数1
    //  arg2    - 参数2
    //@return
    //  int     - 0 成功
    virtual int Initialize(void* arg1, void* arg2) { return 0; }

    //@desc - BaseRpc退出
    //@param
    //  arg1    - 参数1
    //  arg2    - 参数2
    //@return
    virtual void Finalize(void* arg1, void* arg2) { return; }

    //@desc - 设置BaseRpc名称
    //@param
    //  strName - BaseRpc名称
    //@return
    virtual BaseRpc* SetRpcName(const std::string& strName) {
        _rpcName = strName;
        return this;
    }

    //@desc - 设置BaseRpc名称
    //@param
    //  strName - BaseRpc名称
    //@return
    virtual BaseRpc* SetRpcName(std::string&& strName) {
        _rpcName = std::forward<std::string>(strName);
        return this;
    }

    //@desc - 获取BaseRpc名称
    //@param
    //@return
    virtual const std::string& GetRpcName() {
        return _rpcName;
    }

    //@desc - 设置BaseRpc关联route
    //@param
    //  strName - BaseRpc名称
    //@return
    virtual BaseRpc* SetRoute(std::shared_ptr<TITANS::ROUTE::BaseRoute> pRoute) {
        _pRoute = pRoute;
        return this;
    }

    //@desc - 获取BaseRpc关联route
    //@param
    //@return
    virtual std::shared_ptr<TITANS::ROUTE::BaseRoute> GetRoute() {
        return _pRoute;
    }

    //@desc - 设置BaseRpc关联codec
    //@param
    //  strName - BaseRpc名称
    //@return
    virtual BaseRpc* SetCodec(std::shared_ptr<TITANS::CODEC::BaseCodec> pCodec) {
        _pCodec = pCodec;
        return this;
    }

    //@desc - 获取BaseRpc关联codec
    //@param
    //@return
    virtual std::shared_ptr<TITANS::CODEC::BaseCodec> GetCodec() {
        return _pCodec;
    }

    //@desc - 设置BaseRpc关联net
    //@param
    //  strName - BaseRpc名称
    //@return
    virtual BaseRpc* SetNet(std::shared_ptr<TITANS::NET::BaseNet> pNet) {
        _pNet = pNet;
        return this;
    }

    //@desc - 获取BaseRpc关联net
    //@param
    //@return
    virtual std::shared_ptr<TITANS::NET::BaseNet> GetNet() {
        return _pNet;
    }

    //@desc - rpc执行函数
    //@param
    //@return
    //  int     - 0 成功
    virtual int Call() {
        return 0;
    }

    //@desc - 获取日志信息
    //@param
    //@return
    //  std::string     -日志
    virtual std::string GetLogInfo() {
        return _ssLog.str();
    }

    //@desc - 获取日志器
    //@param
    //@return
    //  std::string     -日志器
    virtual std::stringstream& LOG() {
        return _ssLog;
    }

protected:
    //rpc名称
    std::string _rpcName;
    //编码器指针
    std::shared_ptr<TITANS::CODEC::BaseCodec> _pCodec;
    //路由指针
    std::shared_ptr<TITANS::ROUTE::BaseRoute> _pRoute;
    //网络器指针
    std::shared_ptr<TITANS::NET::BaseNet> _pNet;
    //日志器
    std::stringstream _ssLog;
};

}//namespace RPC

}//namespace TITANS

#endif //_TITANS_CORE_RPC_BASERPC_H_