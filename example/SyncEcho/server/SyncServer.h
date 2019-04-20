#ifndef _TITANS_EXAMPLE_SYNCECHO_SERVER_SYNCSERVER_H_
#define _TITANS_EXAMPLE_SYNCECHO_SERVER_SYNCSERVER_H_

#include <core/server/BaseServer.h>

class SyncServer: public TITANS::SERVER::BaseServer {

public:
    SyncServer();

    virtual ~SyncServer();

    virtual int Initialize(std::shared_ptr<TITANS::SERVER::stBaseSvrContext> pContext);

    virtual void Finalize(std::shared_ptr<TITANS::SERVER::stBaseSvrContext> pContext);

    virtual int Serve(std::shared_ptr<TITANS::SERVER::stBaseSvrContext> pContext);


};

#endif //_TITANS_EXAMPLE_SYNCECHO_SERVER_SYNCSERVER_H_