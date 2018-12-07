#ifndef _TITANS_CODEC_CTX_CTXPROTOCODEC_H_
#define _TITANS_CODEC_CTX_CTXPROTOCODEC_H_

#include <core/codec/BaseCodec.h>

namespace TITANS {

namespace CODEC {

template<typename REQ, typename RSP>
class CtxProtoCodec : public BaseCodec {

public:
    CtxProtoCodec() {}

    virtual ~CtxProtoCodec() {}

    virtual int RspPkgEncode(std::string& strData) {
        if (!_bodyRsp.SerializeToString(&strData)) {
            return 4; //ILIVE_EC_INNER_ERROR
        }
        return 0;
    }

    virtual int ReqPkgDecode(const char* pData, const unsigned int len, void* pExt) {
        if (!_bodyReq.ParseFromArray(pData, len)) {
            return 1; //ILIVE_EC_REQ_BAD_PKG
        }
        return 0;
    }

    REQ& BodyReq() {
        return _bodyReq;
    }

    RSP& BodyRsp() {
        return _bodyRsp;
    }

protected:
    REQ _bodyReq;
    RSP _bodyRsp;
};

} //namespace CODEC

} //namespace TITANS

#endif //_TITANS_CODEC_CTX_CTXPROTOCODEC_H_