#include <iostream>
#include <core/codec/ProtoCodec.h>
#include <core/route/IPRoute.h>
#include <core/common/Memory.h>
#include <core/rpc/SyncRpc.h>

#include "../../SyncEcho/client/SyncTcpNet.h"
#include "CoProxySvr.pb.h"

using namespace TITANS::ROUTE;
using namespace TITANS::RPC;
using namespace TITANS::CODEC;
using namespace CoProxySvr;

std::shared_ptr<BaseRpc> GetCoRpcProxy() {

        auto it = std::make_shared<SyncRpc>();
        auto route = std::make_shared<IPRoute>();
        auto net = std::make_shared<SyncTcpNet>();
        route->SetRouteKey("127.0.0.1:1001");
        net->SetTimeout(1000);
        it->SetRpcName("CoProxySvrRpc")
                ->SetNet(net)
                ->SetRoute(route);
        return it;
}

int main() {

    while(true) {

        std::string in;
        ProtoCodec<CoProxyReq, CoProxyRsp> codec;
        std::cin>>in;

        codec.BodyReq().set_data(in);
        codec.BodyReq().set_appid(1001);
        int ret = GetCoRpcProxy()
                ->SetCodec(std::shared_ptr<ProtoCodec<CoProxyReq, CoProxyRsp>>(&codec, TITANS::stack_delete<ProtoCodec<CoProxyReq, CoProxyRsp>>))
                ->Call();

        std::cout<<"ret="<<ret 
                <<",req="<<codec.BodyReq().ShortDebugString()
                <<",rsp="<<codec.BodyRsp().ShortDebugString()
                <<std::endl;
    }
    return 0;
}