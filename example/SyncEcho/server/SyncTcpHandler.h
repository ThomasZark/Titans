#ifndef _TITANS_EXAMPLE_SYNCECHO_SERVER_SYNCTCPHANDLER_H_
#define _TITANS_EXAMPLE_SYNCECHO_SERVER_SYNCTCPHANDLER_H_

#include <core/handler/BaseHandler.h>

class SyncTcpHandler: public TITANS::HANDLER::BaseHandler {

public:
    explicit SyncTcpHandler(unsigned port);

    virtual ~SyncTcpHandler();

    virtual int Initialize(struct stBaseHandlerContext* pContext);

    virtual void Finalize(struct stBaseHandlerContext* pContext);

    virtual int HandleProcess(struct stBaseHandlerContext* pContext);

protected:
    unsigned  _port;

};

#endif //_TITANS_EXAMPLE_SYNCECHO_SERVER_SYNCTCPHANDLER_H_