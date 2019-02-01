#ifndef _TITANS_EXAMPLE_SYNCECHO_SERVER_SYNCTCPHANDLER_H_
#define _TITANS_EXAMPLE_SYNCECHO_SERVER_SYNCTCPHANDLER_H_

#include <core/handler/BaseHandler.h>

class SyncTcpHandler: public TITANS::HANDLER::BaseHandler {

public:
    explicit SyncTcpHandler(unsigned port):_port(port) {};

    virtual ~SyncTcpHandler() {};

    virtual int Initialize(struct TITANS::HANDLER::stBaseHandlerContext* pContext);

    virtual void Finalize(struct TITANS::HANDLER::stBaseHandlerContext* pContext);

    virtual int HandleProcess(struct TITANS::HANDLER::stBaseHandlerContext* pContext);

protected:
    unsigned  _port;

};

#endif //_TITANS_EXAMPLE_SYNCECHO_SERVER_SYNCTCPHANDLER_H_