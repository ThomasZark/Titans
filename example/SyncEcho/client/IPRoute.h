#ifndef _TITANS_EXAMPLE_SYNCECHO_CLIENT_IPROUTR_H_
#define _TITANS_EXAMPLE_SYNCECHO_CLIENT_IPROUTR_H_

#include <core/route/BaseRoute.h>

class IPRoute: public TITANS::ROUTE::BaseRoute {

public:
    IPRoute() {}

    virtual ~IPRoute() {}

    virtual int GetRoute(const std::string& strRouteKey, struct TITANS::ROUTE::stRoute_t& stRoute) {
        auto iter = strRouteKey.find(":");
        if(iter == std::string::npos) {
            return TITANS::ROUTE::ROUTE_KEY_ERROR;
        }
        stRoute.ip = strRouteKey.substr(0, iter);
        stRoute.port = static_cast<unsigned>(atoi(strRouteKey.substr(++iter).c_str()));
        return TITANS::ROUTE::ROUTE_SUCC;
    }

};

#endif //_TITANS_EXAMPLE_SYNCECHO_CLIENT_IPROUTR_H_