#ifndef _TITANS_CORE_NET_BASENET_H_
#define _TITANS_CORE_NET_BASENET_H_

#include <string>
#include <functional>

namespace TITANS {

namespace NET {

enum {
    NET_SUCC            = 0,
};

class BaseNet {

public:
    typedef std::function<int(const char*, const unsigned int, void*)> CheckFunc;

public:
    BaseNet() {}

    virtual ~BaseNet() {}

    virtual BaseNet* SetStrReq(std::string& strReq) {
        _strReq = strReq;
        return this;
    }

    virtual std::string& GetStrRsp() {
        return _strRsp;
    }

    virtual BaseNet* SetIP(std::string& ip) {
        _ip = ip;
        return this;
    }

    virtual BaseNet* SetPort(unsigned port) {
        _port = port;
        return this;
    }

    virtual BaseNet* SetTimeout(unsigned timeout) {
        _timeout = timeout;
        return this;
    }

    virtual BaseNet* SetCheckFunc(CheckFunc checkFunc) {
        _checkFunc = checkFunc;
        return this;
    }

    virtual int DoNetProcess() {
        return 0;
    }

protected:
    std::string _strReq;
    std::string _strRsp;
    std::string _ip;
    unsigned _port;
    unsigned _timeout;

    CheckFunc _checkFunc;
};

} //namespace CODEC

} //namespace TITANS

#endif //_TITANS_CORE_NET_BASENET_H_