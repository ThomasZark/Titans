#ifndef _TITANS_CORE_NET_BASENET_H_
#define _TITANS_CORE_NET_BASENET_H_

#include <string>
#include <sstream>
#include <functional>
#include <memory>   

namespace TITANS {

namespace NET {

enum {
    NET_SUCC            = 0,
    NET_CONNECT_ERROR   = -50001,
};

class BaseNet {

public:
    typedef std::function<int(const char*, const unsigned int, void*)> CheckFunc;

public:
    BaseNet()
    :_timeout(2000) {}

    virtual ~BaseNet() {}

    //@desc - 设置请求数据
    //@param
    //  pData  - 请求包数据指针
    //@return
    // BaseNet*
    virtual BaseNet* SetReqData(std::shared_ptr<std::string>pData) {
        _pReq = pData;
        return this;
    }

    //@desc - 设置请求数据(拷贝数据)
    //@param
    //  strData  - 请求包数据
    //@return
    // BaseNet*
    virtual BaseNet* SetReqData(const std::string& strData) {
        _pReq = std::make_shared<std::string>(strData.c_str());
        return this;
    }

    //@desc - 获取响应数据
    //@param
    //@return
    // const std::string& 响应数据
    virtual std::string& GetRspData() {
        return _strRsp;
    }

    //@desc - 设置目标IP
    //@param
    //  ip  - ip地址
    //@return
    // BaseNet*
    virtual BaseNet* SetIP(const std::string& ip) {
        _ip = ip;
        return this;
    }

    //@desc - 设置目标IP
    //@param
    //  ip  - ip地址
    //@return
    // BaseNet*
    virtual BaseNet* SetIP(std::string&& ip) {
        _ip = std::forward<std::string>(ip);
        return this;
    }

    //@desc - 设置目标port
    //@param
    //  port  - 目标port
    //@return
    // BaseNet*
    virtual BaseNet* SetPort(unsigned port) {
        _port = port;
        return this;
    }

    //@desc - 设置超时时间
    //@param
    //  timeout  - 超时时间
    //@return
    // BaseNet*
    virtual BaseNet* SetTimeout(unsigned timeout) {
        _timeout = timeout;
        return this;
    }

    //@desc - 设置包检查函数
    //@param
    //  CheckFunc  - 包检查函数
    //@return
    // BaseNet*
    virtual BaseNet* SetCheckFunc(CheckFunc checkFunc) {
        _checkFunc = checkFunc;
        return this;
    }

    //@desc -  执行网络操作
    //@param
    //@return
    // int 0 succ
    virtual int DoNetProcess() {
        return 0;
    }

    //@desc - 获取日志信息
    //@param
    //@return
    //  std::string     -日志
    virtual std::string GetErrMsg() {
        return _ssLog.str();
    }

    //@desc - 获取错误记录器
    //@param
    //@return
    //  std::string     -记录器
    virtual std::stringstream& ERROR() {
        return _ssLog;
    }

protected:
    //请求包数据指针
    std::shared_ptr<std::string>_pReq;
    //响应包数据
    std::string _strRsp;
    //ip地址
    std::string _ip;
    //端口号
    unsigned _port;
    //超时时间, 默认2s
    unsigned _timeout;
    //响应包检查函数
    CheckFunc _checkFunc;
    //记录器
    std::stringstream _ssLog;
};

} //namespace CODEC

} //namespace TITANS

#endif //_TITANS_CORE_NET_BASENET_H_