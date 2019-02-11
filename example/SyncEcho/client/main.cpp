#include <iostream>
#include <core/route/IPRoute.h>
#include <core/common/Memory.h>
#include <core/rpc/SyncRpc.h>

#include "PlatCodec.h"
#include "SyncTcpNet.h"

using namespace TITANS::ROUTE;
using namespace TITANS::RPC;

std::shared_ptr<BaseRpc> GetEchoRpcProxy() {

        auto it = std::make_shared<SyncRpc>();
        auto route = std::make_shared<IPRoute>();
        auto net = std::make_shared<SyncTcpNet>();
        route->SetRouteKey("127.0.0.1:1000");
        net->SetTimeout(1000);
        it->SetRpcName("SyncEchoRpc")
                ->SetNet(net)
                ->SetRoute(route);
        return it;
}

int main() {

    while(true) {

        PlatCodec codec;
        std::cin>>codec.BodyReq();
        int ret = GetEchoRpcProxy()
                ->SetCodec(std::shared_ptr<PlatCodec>(&codec, TITANS::stack_delete<PlatCodec>))
                ->Call();

        std::cout<<"ret="<<ret 
                <<"rsp="<<codec.BodyRsp()
                <<std::endl;
    }
    return 0;
}