#ifndef _TITANS_CORE_RPC_STREAMRPC_H_
#define _TITANS_CORE_RPC_STREAMRPC_H_

#include <core/rpc/BaseRpc.h>

#include <chrono>  

namespace TITANS {

namespace RPC {

class ReqStreamRpc: public BaseRpc {

public:
    ReqStreamRpc() {}

    virtual ~ReqStreamRpc() {}

    virtual int Call() {
        CHECK_RPC_PARAM(GetCodec())
        CHECK_RPC_PARAM(GetRoute())
        CHECK_RPC_PARAM(GetNet())    

        std::shared_ptr<std::string> pReq = std::make_shared<std::string>();
        int ret = GetCodec()->ReqPkgEncode(*pReq);
        if(TITANS::CODEC::CODEC_SUCC != ret) {
            LOG()<< "Req Codec failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_CODEC_ERROR;
        }     

        TITANS::ROUTE::stRoute_t route;    
        ret = GetRoute()->GetRoute(route);
        if(TITANS::ROUTE::ROUTE_SUCC != ret) {
            LOG()<< "Req Route failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_ROUTE_RERROR;
        }

        auto start = std::chrono::system_clock::now();
        auto checkFunc = std::bind(&TITANS::CODEC::BaseCodec::RspPkgCheck, 
                                    GetCodec(), 
                                    std::placeholders::_1,
                                    std::placeholders::_2,
                                    std::placeholders::_3);
        ret = GetNet()->SetIP(route.ip)
                        ->SetPort(route.port)
                        ->SetReqData(pReq)
                        ->SetCheckFunc(checkFunc)
                        ->DoNetProcess();
        
        route.result = ret;
        route.cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
        GetRoute()->UpdateRoute(route);

        if(TITANS::NET::NET_SUCC != ret) {
            LOG()<< "Req Net failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_NET_ERROR;
        }   
    }

    virtual int Complete() {

        const std::string& strRsp = GetNet()->GetRspData();
        ret = GetCodec()->RspPkgDecode(strRsp.c_str(), strRsp.size(), nullptr);
        if(TITANS::CODEC::CODEC_SUCC != ret) {
            LOG()<< "Rsp Codec failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_CODEC_ERROR;
        }
        return TITANS::RPC::RPC_SUCC;
    }
};

class RspStreamRpc: public BaseRpc {

public:
    RspStreamRpc() {}

    virtual ~RspStreamRpc() {}

    virtual int Call() {
        CHECK_RPC_PARAM(GetCodec())
        CHECK_RPC_PARAM(GetRoute())
        CHECK_RPC_PARAM(GetNet())    

        std::shared_ptr<std::string> pReq = std::make_shared<std::string>();
        int ret = GetCodec()->ReqPkgEncode(*pReq);

        if(TITANS::CODEC::CODEC_SUCC != ret
            && TITANS::CODEC::REQ_PKG_ENCODE_EMPTY != ret) {
            LOG()<< "Req Codec failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_CODEC_ERROR;
        }

        TITANS::ROUTE::stRoute_t route;    
        ret = GetRoute()->GetRoute(route);
        if(TITANS::ROUTE::ROUTE_SUCC != ret) {
            LOG()<< "Req Route failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_ROUTE_RERROR;
        }

        auto start = std::chrono::system_clock::now();
        auto checkFunc = std::bind(&TITANS::CODEC::BaseCodec::RspPkgCheck, 
                                    GetCodec(), 
                                    std::placeholders::_1,
                                    std::placeholders::_2,
                                    std::placeholders::_3);
        ret = GetNet()->SetIP(route.ip)
                        ->SetPort(route.port)
                        ->SetReqData(pReq)
                        ->SetCheckFunc(checkFunc)
                        ->DoNetProcess();
        
        route.result = ret;
        route.cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
        GetRoute()->UpdateRoute(route);

        if(TITANS::NET::NET_SUCC != ret) {
            LOG()<< "Req Net failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_NET_ERROR;
        }   
        
        const std::string& strRsp = GetNet()->GetRspData();
        ret = GetCodec()->RspPkgDecode(strRsp.c_str(), strRsp.size(), nullptr);
        if(TITANS::CODEC::CODEC_SUCC != ret) {
            LOG()<< "Rsp Codec failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_CODEC_ERROR;
        }
        return TITANS::RPC::RPC_SUCC;
    }

    virtual int Complete() {

        const std::string& strRsp = GetNet()->GetRspData();
        ret = GetCodec()->RspPkgDecode(strRsp.c_str(), strRsp.size(), nullptr);
        if(TITANS::CODEC::CODEC_SUCC != ret) {
            LOG()<< "Rsp Codec failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_CODEC_ERROR;
        }
        return TITANS::RPC::RPC_SUCC;
    }
};

}//namespace RPC

}//namespace TITANS

#endif //_TITANS_CORE_RPC_STREAMRPC_H_