#include <iostream>

#include "EchoRpc.h"
#include "IPRoute.h"
#include "PlatCodec.h"
#include "SyncTcpNet.h"

int main() {

    EchoRpc rpc;
    IPRoute route;
    SyncTcpNet net;

    while(true) {

        PlatCodec codec;
        std::cin>>codec.BodyReq();
        int ret = rpc.SetCodec(&codec)
                        ->SetNet(&net)
                        ->SetRoute(&route)
                        ->Call();

        std::cout<<"ret="<<ret 
                <<"rsp="<<codec.BodyRsp()
                <<std::endl;
    }
    return 0;
}