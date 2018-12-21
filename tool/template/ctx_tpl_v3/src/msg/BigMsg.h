#ifndef {{UP_SRV_NAME}}_SRC_MSG_{{UP_SRV_NAME}}_MSG_H
#define {{UP_SRV_NAME}}_SRC_MSG_{{UP_SRV_NAME}}_MSG_H

#include <codec/ctx/CtxProtoCodec.h>
#include <spp/handler/ctx/ilive_ctx/IliveMsg.h>

template<typename REQ, typename RSP>
class {{CAMEL_SRV_NAME}}Msg: public TITANS::HANDLER::IliveMsg {

public:
    {{CAMEL_SRV_NAME}}Msg() {
        SetBodyCodec(&_bodyCodec);
    }

    virtual ~{{CAMEL_SRV_NAME}}Msg() {

    }

protected:
    REQ& BodyReq() { return _bodyCodec.BodyReq();}
    RSP& BodyRsp() { return _bodyCodec.BodyRsp();}

    TITANS::CODEC::CtxProtoCodec<REQ, RSP> _bodyCodec;
};

#endif //{{UP_SRV_NAME}}_SRC_MSG_{{UP_SRV_NAME}}_MSG_H