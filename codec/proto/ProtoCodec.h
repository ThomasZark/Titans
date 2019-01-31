#ifndef _TITANS_CODEC_PROTO_PROTOCODEC_H_
#define _TITANS_CODEC_PROTO_PROTOCODEC_H_

#include <core/codec/BaseCodec.h>

namespace TITANS {

namespace CODEC {

template<typename REQ, typename RSP>
class ProtoCodec : public BaseCodec {

public:
    ProtoCodec() {}

    virtual ~ProtoCodec() {}

    virtual int ReqPkgEncode(std::string& strData) {
        if (!_bodyReq.SerializeToString(&strData)) {
            return REQ_PKG_ENCODE_ERROR;
        }
        return CODEC_SUCC;
    }

    virtual int ReqPkgDecode(const char* pData, const unsigned int len, void* pExt) {
        if (!_bodyReq.ParseFromArray(pData, len)) {
            return REQ_PKG_DECODE_ERROR;
        }
        return CODEC_SUCC;
    }

    virtual int RspPkgEncode(std::string& strData) {
        if (!_bodyRsp.SerializeToString(&strData)) {
            return RSP_PKG_ENCODE_ERROR;
        }
        return CODEC_SUCC;
    }

    virtual int RspPkgDecode(const char* pData, const unsigned int len, void* pExt) {
        if (!_bodyRsp.ParseFromArray(pData, len)) {
            return RSP_PKG_DECODE_ERROR;
        }
        return CODEC_SUCC;
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

#endif //_TITANS_CODEC_PROTO_PROTOCODEC_H_