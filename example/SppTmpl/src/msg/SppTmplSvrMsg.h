#ifndef _SPP_TMPL_SVR_MSG_H_
#define _SPP_TMPL_SVR_MSG_H_

#include <codec/ctx/CtxProtoCodec.h>
#include <spp/handler/ctx/ilive_ctx/IliveMsg.h>

template<typename REQ, typename RSP>
class SppTmplSvrMsg: public TITANS::HANDLER::IliveMsg {

public:
    SppTmplSvrMsg() {
        SetBodyCodec(&_bodyCodec);
    }

    virtual ~SppTmplSvrMsg() {

    }

    int GetRetcode(int subcmd, int param) {return subcmd + param;}

protected:
    REQ& BodyReq() { return _bodyCodec.BodyReq();}
    RSP& BodyRsp() { return _bodyCodec.BodyRsp();}

    TITANS::CODEC::CtxProtoCodec<REQ, RSP> _bodyCodec;
};

#endif //_SPP_TMPL_SVR_MSG_H_