#ifndef _TITANS_EXAMPLE_COPROXY_SERVER_COPROXYSERVER_H_
#define _TITANS_EXAMPLE_COPROXY_SERVER_COPROXYSERVER_H_

#include <core/server/BaseServer.h>
#include <iostream>

using namespace TITANS::SERVER;

class CoProxyServer: public BaseServer {

public:
    CoProxyServer() {}

    virtual ~CoProxyServer() {}

    virtual int Initialize(std::shared_ptr<stBaseSvrContext> pContext) {
        std::cout<<GetServerName()<<" start!"<<std::endl;
        return 0;
    }

    virtual void Finalize(std::shared_ptr<stBaseSvrContext> pContext) {
        std::cout<<GetServerName()<<" end!"<<std::endl;
    }

    virtual int Serve(std::shared_ptr<stBaseSvrContext> pContext) {
        return GetHandler()->HandleProcess(nullptr);
    }
};



#endif //_TITANS_EXAMPLE_COPROXY_SERVER_COPROXYSERVER_H_