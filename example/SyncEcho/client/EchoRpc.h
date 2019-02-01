#ifndef _TITANS_EXAMPLE_SYNCECHO_CLIENT_ECHORPC_H_
#define _TITANS_EXAMPLE_SYNCECHO_CLIENT_ECHORPC_H_

#include <core/rpc/BaseRpc.h>

#define CHECK_PARAM(pParam) \
    if(NULL == pParam) {    \
        _ssLog<< #pParam <<" is NULL"<<std::endl; \
        return TITANS::RPC::RPC_PARAM_ERROR; \
    }   \

class EchoRpc: public TITANS::RPC::BaseRpc {

public:
    EchoRpc() {}

    virtual ~EchoRpc() {}

    virtual int Call() {
        CHECK_PARAM(GetCodec())
        CHECK_PARAM(GetRoute())
        CHECK_PARAM(GetNet())

        std::string strReq;
        int ret = GetCodec()->ReqPkgEncode(strReq);
        if(TITANS::CODEC::CODEC_SUCC != ret) {
            _ssLog<< "Req Codec failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_CODEC_ERROR;
        }

        TITANS::ROUTE::stRoute_t route;
        std::string routeKey = "127.0.0.1:1000";
        ret = GetRoute()->GetRoute(routeKey, route);
        if(TITANS::ROUTE::ROUTE_SUCC != ret) {
            _ssLog<< "Req Route failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_ROUTE_RERROR;
        }

        ret = GetNet()->SetIP(route.ip)
                        ->SetPort(route.port)
                        ->SetTimeout(1000)
                        ->SetStrReq(strReq)
                        ->SetCheckFunc(std::bind(&TITANS::CODEC::BaseCodec::RspPkgCheck, GetCodec(), std::placeholders::_1,std::placeholders::_2,std::placeholders::_3))
                        ->DoNetProcess();
        if(TITANS::NET::NET_SUCC != ret) {
            _ssLog<< "Req Net failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_NET_ERROR;
        }  

        route.result = ret;
        GetRoute()->UpdateRoute("127.0.0.1:1000", route);

        const std::string& strRsp = GetNet()->GetStrRsp();
        ret = GetCodec()->RspPkgDecode(strRsp.c_str(), strRsp.size(), NULL);
        if(TITANS::CODEC::CODEC_SUCC != ret) {
            _ssLog<< "Rsp Codec failed, ret="<<ret<<std::endl;
            return TITANS::RPC::RPC_CODEC_ERROR;
        }

        return TITANS::RPC::RPC_SUCC;
    }

};

#endif //_TITANS_EXAMPLE_SYNCECHO_CLIENT_ECHORPC_H_