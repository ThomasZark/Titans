#ifndef _TITANS_EXAMPLE_COPROXY_SERVER_COTCPHANDLER_H_
#define _TITANS_EXAMPLE_COPROXY_SERVER_COTCPHANDLER_H_

#include <core/codec/ProtoCodec.h>
#include <core/handler/BaseHandler.h>
#include <core/route/IPRoute.h>
#include <core/common/Memory.h>
#include <core/rpc/SyncRpc.h>

#include "../../SyncEcho/client/PlatCodec.h"
#include "../../SyncEcho/client/SyncTcpNet.h"
#include "CoProxySvr.pb.h"

#include "libco/co_routine.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <stack>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;
using namespace TITANS::HANDLER;
using namespace TITANS::ROUTE;
using namespace TITANS::RPC;
using namespace TITANS::CODEC;
using namespace CoProxySvr;

static int SetNonBlock(int iSock){
    int iFlags;

    iFlags = fcntl(iSock, F_GETFL, 0);
    iFlags |= O_NONBLOCK;
    iFlags |= O_NDELAY;
    int ret = fcntl(iSock, F_SETFL, iFlags);
    return ret;
}

static std::shared_ptr<BaseRpc> GetEchoRpcProxy() {

    auto rpc = std::make_shared<SyncRpc>();
    auto route = std::make_shared<IPRoute>();
    auto net = std::make_shared<SyncTcpNet>();
    route->SetRouteKey("127.0.0.1:1000");
    net->SetTimeout(1000);
    rpc->SetRpcName("SyncEchoRpc")
            ->SetNet(net)
            ->SetRoute(route);
    return rpc;
}

static std::string handle_process(const char* buf, unsigned size) {

    ProtoCodec<CoProxyReq, CoProxyRsp> proxyCodec;
    PlatCodec echoCodec;

    proxyCodec.ReqPkgDecode(buf, size, nullptr);
    echoCodec.BodyReq() = proxyCodec.BodyReq().data();
    int ret = GetEchoRpcProxy()
            ->SetCodec(std::shared_ptr<PlatCodec>(&echoCodec, TITANS::stack_delete<PlatCodec>))
            ->Call();

    std::cout<<"ret="<<ret
            <<".rsp="<<echoCodec.BodyRsp()
            <<std::endl;
    
    proxyCodec.BodyRsp().set_retcode(1);
    proxyCodec.BodyRsp().set_errmsg(echoCodec.BodyRsp());
    std::string strRsp;
    proxyCodec.RspPkgEncode(strRsp);
    return strRsp;
}

static void *readwrite_routine( void *arg ) {

    int fd = *((int*)arg);
    char buf[ 1024 * 16 ];
    struct pollfd pf = { 0 };
    
    for(;;) {
        pf.fd = fd;
        pf.events = (POLLIN|POLLERR|POLLHUP);
        co_poll( co_get_epoll_ct(),&pf,1,1000);

        int ret = read( fd,buf,sizeof(buf) );
        if( ret > 0 ) {
            std::string rsp = handle_process(buf, ret);
            ret = write( fd, rsp.c_str(), rsp.size());
        }
        if( ret <= 0 )
        {
            // accept_routine->SetNonBlock(fd) cause EAGAIN, we should continue
            if (errno == EAGAIN)
                continue;
            close( fd );
            break;
        }
    }

	return 0;
}

int co_accept(int fd, struct sockaddr *addr, socklen_t *len );
static void *accept_routine( void * arg) {

    int listen_fd = *((int*)arg);
	printf("accept_routine\n");
	fflush(stdout); 
	for(;;) {
		struct sockaddr_in addr; //maybe sockaddr_un;
		memset( &addr,0,sizeof(addr) );
		socklen_t len = sizeof(addr);

		int fd = co_accept(listen_fd, (struct sockaddr *)&addr, &len);
		if( fd < 0 )
		{
			struct pollfd pf = { 0 };
			pf.fd = listen_fd;
			pf.events = (POLLIN|POLLERR|POLLHUP);
			co_poll( co_get_epoll_ct(),&pf,1,1000 );
			continue;
		}

		SetNonBlock( fd );
        
        stCoRoutine_t *readwrite_co = NULL;
		co_create( &readwrite_co, NULL, readwrite_routine, (void*)&fd );
		co_resume( readwrite_co );
	}
	return 0;
}

class CoTcpHandler: public BaseHandler {

public:
    explicit CoTcpHandler(unsigned port):_port(port) {};

    virtual ~CoTcpHandler() {};

    virtual int Initialize(std::shared_ptr<stBaseHandlerContext> pContext) {
        std::cout<<"CoTcpHandler init"<<std::endl;
        return 0;
    }

    virtual void Finalize(std::shared_ptr<stBaseHandlerContext> pContext){
        std::cout<<"CoTcpHandler final"<<std::endl;
    }

    virtual int HandleProcess(std::shared_ptr<stBaseHandlerContext> pContext){
    
        co_enable_hook_sys();

        int sockfd;
        struct sockaddr_in server_addr;

        if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
            fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
            return 0;
        }

        bzero(&server_addr,sizeof(struct sockaddr_in));
        server_addr.sin_family=AF_INET;
        server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        server_addr.sin_port=htons(_port);
        if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1){
            fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
            return 0;
        }  
        if(listen(sockfd,1024)==-1){
            fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
            return 0;
        }

        fprintf(stdout,"Listen port=%u\n", _port);
        SetNonBlock( sockfd );

		stCoRoutine_t *accept_co = NULL;
		co_create( &accept_co,NULL,accept_routine, (void*)&sockfd );
		co_resume( accept_co );
		co_eventloop( co_get_epoll_ct(),0,0 );

        close(sockfd);
        return 0;
    }


protected:
    unsigned  _port;
};

#endif //_TITANS_EXAMPLE_COPROXY_SERVER_COTCPHANDLER_H_