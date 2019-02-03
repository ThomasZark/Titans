#ifndef _TITANS_CORE_ROUTE_IPROUTR_H_
#define _TITANS_CORE_ROUTE_IPROUTR_H_

#include <core/common/Singleton.h>
#include <core/route/BaseRoute.h>

namespace TITANS {

namespace ROUTE {

class IPRoute: public BaseRoute {

public:
    IPRoute() {}

    virtual ~IPRoute() {}

    virtual int GetRoute(const std::string& strRouteKey, struct stRoute_t& stRoute) {
        auto iter = strRouteKey.find(":");
        if(iter == std::string::npos) {
            return ROUTE_KEY_ERROR;
        }
        stRoute.ip = strRouteKey.substr(0, iter);
        stRoute.port = static_cast<unsigned>(atoi(strRouteKey.substr(++iter).c_str()));
        return ROUTE_SUCC;
    }

};

typedef TITANS::Singleton<IPRoute> sIPRoute;

} // namespace ROUTE

} // namepace TITANS

#endif //_TITANS_CORE_ROUTE_IPROUTR_H_