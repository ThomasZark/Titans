#include <iostream>
#include <core/route/IPRoute.h>
#include <core/common/Memory.h>

#include "EchoRpc.h"
#include "PlatCodec.h"
#include "SyncTcpNet.h"

using namespace TITANS::ROUTE;
using namespace TITANS::RPC;

std::shared_ptr<BaseRpc> GetEchoRpcProxy() {

        auto it = std::make_shared<EchoRpc>();
        it->SetNet(std::make_shared<SyncTcpNet>())
                ->SetRoute(std::shared_ptr<IPRoute>(sIPRoute::Instance(), TITANS::stack_delete<IPRoute>));
        return it;
}

int main() {

    while(true) {

        PlatCodec codec;
        std::cin>>codec.BodyReq();
        int ret = GetEchoRpcProxy()->SetCodec(std::shared_ptr<PlatCodec>(&codec, TITANS::stack_delete<PlatCodec>))->Call();

        std::cout<<"ret="<<ret 
                <<"rsp="<<codec.BodyRsp()
                <<std::endl;
    }
    return 0;
}