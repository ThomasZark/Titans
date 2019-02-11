#ifndef _TITANS_CORE_CODEC_BASEROUTE_H_
#define _TITANS_CORE_CODEC_BASEROUTE_H_

#include <string>
#include <sstream>

namespace TITANS {

namespace ROUTE {

enum {
    ROUTE_SUCC          = 0,
    ROUTE_KEY_ERROR     = -20001,
    ROUTE_GET_ERROR     = -20002,
    ROUTE_UPDATE_ERROR  = -20003,
    ROUTE_PROTECT       = -20004,
};

struct stRoute_t {
    std::string ip;
    unsigned port;
    unsigned cost;
    int result;
    void* ext;
};

class BaseRoute {

public:
    BaseRoute() {}

    virtual ~BaseRoute() {}

    //@desc - 获取路由地址
    //@param
    //  stRoute  - 路由信息
    //@return
    // int   - 0 成功
    virtual int GetRoute(struct stRoute_t& stRoute) {
        return 0;
    }

    //@desc - 路由地址上报
    //@param
    //  stRoute  - 路由信息
    //@return
    // int   - 0 成功
    virtual int UpdateRoute(const struct stRoute_t& stRoute) {
        return 0;
    }

    //@desc - 设置BaseRoute Key
    //@param
    //  strKey - BaseRoute Key
    //@return
    virtual BaseRoute* SetRouteKey(const std::string& strKey) {
        _strRouteKey = strKey;
        return this;
    }

    //@desc - 设置BaseRoute Key
    //@param
    //  strKey - BaseRoute Key
    //@return
    virtual BaseRoute* SetRouteKey(std::string&& strKey) {
        _strRouteKey = std::forward<std::string>(strKey);
        return this;
    }

    //@desc - 获取BaseRoute Key
    //@param
    //@return
    virtual const std::string& GetRouteKey() {
        return _strRouteKey;
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
    //路由key
    std::string _strRouteKey;
    //日志器
    std::stringstream _ssLog;
};

} // namespace ROUTE

} // namepace TITANS

#endif //_TITANS_CORE_CODEC_BASEROUTE_H_