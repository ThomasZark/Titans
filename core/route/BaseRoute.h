#ifndef _TITANS_CORE_CODEC_BASEROUTE_H_
#define _TITANS_CORE_CODEC_BASEROUTE_H_

#include <string>

namespace TITANS {

namespace ROUTE {

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
    //  strRouteKey  - 路由key
    //  stRoute  - 返回路由信息
    //@return
    // int   - 0 成功
    virtual int GetRoute(const std::string& strRouteKey, struct stRoute_t& stRoute) {
        return 0;
    }

    //@desc - 路由地址上报
    //@param
    //  strRouteKey  - 路由key
    //  stRoute  - 路由信息
    //@return
    // int   - 0 成功
    virtual int UpdateRoute(const std::string& strRouteKey, const struct stRoute_t& stRoute) {
        return 0;
    }
};

} // namespace ROUTE

} // namepace TITANS

#endif //_TITANS_CORE_CODEC_BASEROUTE_H_