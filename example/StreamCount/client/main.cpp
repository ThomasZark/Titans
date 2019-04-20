#include <iostream>
#include <core/rpc/StreamRpc.h>

#include "StreamCountSvr.pb.h"

using namespace TITANS::RPC;
using namespace StreamCountSvr;


std::shared_ptr<BaseRpc> GetStreamCountProxy() {

        auto it = std::make_shared<StreamRpc>();
        return it;
}

int main() {

    StreamCodec<CountReq, CountRsp> codec;
    auto streamRpc = GetStreamCountProxy();
    for(int i=0; i< 10; i++) {
        int ret = streamRpc->SetCodec(std::shared_ptr<StreamCodec<CountReq, CountRsp>>(&codec, TITANS::stack_delete<StreamCodec<CountReq, CountRsp>>))
                            ->StreamNext();
        
        std::cout<<"ret="<<ret 
                <<",req="<<codec.BodyReq().ShortDebugString()
                <<",rsp="<<codec.BodyRsp().ShortDebugString()
                <<std::endl;
    }

    streamRpc->StreamComplete();
}