#ifndef _TITANS_CORE_RPC_BASERPC_H_
#define _TITANS_CORE_RPC_BASERPC_H_

#include <string>
#include <sstream>

#include <core/codec/BaseCodec.h>
#include <core/route/BaseRoute.h>
#include <core/net/BaseNet.h>

namespace TITANS {

namespace RPC {

class BaseRpc {

public:
    BaseRpc()
    :_pCodec(NULL)
    ,_pRoute(NULL)
    ,_pNet(NULL) {}

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
    virtual BaseRpc* SetRpcName(std::string& strName) {
        _pluginName = strName;
        return this;
    }

    //@desc - 获取BaseRpc名称
    //@param
    //@return
    virtual const std::string& GetRpcName() {
        return _pluginName;
    }

    //@desc - 设置BaseRpc关联route
    //@param
    //  strName - BaseRpc名称
    //@return
    virtual BaseRpc* SetRoute(TITANS::ROUTE::BaseRoute* pRoute) {
        _pRoute = pRoute;
        return this;
    }

    //@desc - 获取BaseRpc关联route
    //@param
    //@return
    virtual TITANS::ROUTE::BaseRoute* GetRoute() {
        return _pRoute;
    }

    //@desc - 设置BaseRpc关联codec
    //@param
    //  strName - BaseRpc名称
    //@return
    virtual BaseRpc* SetCodec(TITANS::CODEC::BaseCodec* pCodec) {
        _pCodec = pCodec;
        return this;
    }

    //@desc - 获取BaseRpc关联codec
    //@param
    //@return
    virtual TITANS::CODEC::BaseCodec* GetCodec() {
        return _pCodec;
    }

    //@desc - 设置BaseRpc关联net
    //@param
    //  strName - BaseRpc名称
    //@return
    virtual BaseRpc* SetNet(TITANS::NET::BaseNet* pNet) {
        _pNet = pNet;
        return this;
    }

    //@desc - 获取BaseRpc关联net
    //@param
    //@return
    virtual TITANS::NET::BaseNet* GetNet() {
        return _pNet;
    }

    //@desc - 获取日志信息
    //@param
    //@return
    //  std::string     - 0 日志
    virtual std::string GetLogInfo() {
        return _ssLog.str();
    }

protected:
    std::stringstream _ssLog;
    std::string _pluginName;

    TITANS::CODEC::BaseCodec* _pCodec;
    TITANS::ROUTE::BaseRoute* _pRoute;
    TITANS::NET::BaseNet* _pNet;
};

}//namespace RPC

}//namespace TITANS

#endif //_TITANS_CORE_RPC_BASERPC_H_