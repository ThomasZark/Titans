#ifndef _TITANS_EXAMPLE_SYNCECHO_CLIENT_SYNCTCPNET_H_
#define _TITANS_EXAMPLE_SYNCECHO_CLIENT_SYNCTCPNET_H_

#include <core/net/BaseNet.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

class SyncTcpNet: public TITANS::NET::BaseNet {

public:
    SyncTcpNet() {}

    virtual ~SyncTcpNet() {}

    virtual int DoNetProcess() {

        struct sockaddr_in servaddr;
        char buf[4096];

        int sockfd,n;

        sockfd = socket(AF_INET,SOCK_STREAM,0);

        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        inet_pton(AF_INET,_ip.c_str(),&servaddr.sin_addr);
        servaddr.sin_port = htons(_port);

        if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
            printf("connet error:%s\n",strerror(errno));
        }

        write(sockfd, _pReq->c_str(), _pReq->size()); 

        _strRsp.clear();
        while(true) {
            n = read(sockfd,buf,4096);
            printf("Recv data is %s n is %d\n",buf, n);
            _strRsp.append(buf, n);
            if(_checkFunc(_strRsp.c_str(), _strRsp.size(), nullptr) > 0) {
                break;
            }
        };

        close(sockfd);
        return 0;
    }
};

#endif //_TITANS_EXAMPLE_SYNCECHO_CLIENT_SYNCTCPNET_H_