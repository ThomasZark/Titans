#ifndef {{UP_SRV_NAME}}_SRC_MSG_{{UP_SRV_NAME}}_MSG_H
#define {{UP_SRV_NAME}}_SRC_MSG_{{UP_SRV_NAME}}_MSG_H

#include <codec/ctx/CtxProtoCodec.h>
#include <spp/handler/ctx/ilive_ctx/IliveMsg.h>

template<typename REQ_TYPE, typename RSP_TYPE>
class {{CAMEL_SRV_NAME}}Msg: public TITANS::HANDLER::IliveMsg {

public:
    {{CAMEL_SRV_NAME}}Msg() {
        SetBodyCodec(&_bodyCodec);
    }

    virtual ~{{CAMEL_SRV_NAME}}Msg() {

    }

protected:
    REQ_TYPE& BodyReq() { return _bodyCodec.BodyReq();}
    RSP_TYPE& BodyRsp() { return _bodyCodec.BodyRsp();}

    TITANS::CODEC::CtxProtoCodec<REQ_TYPE, RSP_TYPE> _bodyCodec;
};

#endif //{{UP_SRV_NAME}}_SRC_MSG_{{UP_SRV_NAME}}_MSG_H