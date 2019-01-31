#include "SyncTcpHandler.h"

#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<iostream>

using namespace TITANS::HANDLER;

SyncTcpHandler::SyncTcpHandler():_port(port) {

}

SyncTcpHandler::~SyncTcpHandler() {

}

int SyncTcpHandler::Initialize(struct stBaseHandlerContext* pContext) {
    std::cout<<"SyncTcpHandler init"<<std::endl;
}

int SyncTcpHandler::HandleProcess(struct stBaseHandlerContext* pContext) {
    
    int sockfd,new_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sin_size,iDataNum;
    char buffer[4096];

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
    if(listen(sockfd,5)==-1){
        fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
        return 0;
    }

    while(1)
    {
        sin_size=sizeof(struct sockaddr_in);
        if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)
        {
            fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
            return 0;
        }
        fprintf(stdout,"Server get connection from %s\n",inet_ntoa(client_addr.sin_addr));
        iDataNum=recv(new_fd,buffer,4096,0);
        if(iDataNum<0)
        { 
            perror("Recv\n");
            exit(1);
        }
        printf("Recv data is %s\n",buffer);
        send(new_fd,buffer,sizeof(buffer),0);
        close(new_fd);
    }

    close(sockfd);
    return 0;
}

void SyncTcpHandler::Finalize(struct stBaseHandlerContext* pContext) {
    std::cout<<"SyncTcpHandler final"<<std::endl;
}