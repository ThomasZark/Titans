#ifndef _TITANS_EXAMPLE_SYNCECHO_CLIENT_PLATCODEC_H_
#define _TITANS_EXAMPLE_SYNCECHO_CLIENT_PLATCODEC_H_

#include <core/codec/BaseCodec.h>

class PlatCodec: public TITANS::CODEC::BaseCodec {

public:
    PlatCodec() {}

    virtual ~PlatCodec() {}

    virtual int ReqPkgEncode(std::string& strData) {
        strData = _strReq;
        return 0;
    }

    virtual int ReqPkgDecode(const char* pData, const unsigned int len, void* pExt) {
        _strReq.assign(pData, len);
        return 0;
    }

    virtual int RspPkgEncode(std::string& strData) {
        strData = _strRsp;
        return 0;
    }

    virtual int RspPkgDecode(const char* pData, const unsigned int len, void* pExt) {
        _strRsp.assign(pData, len);
        return 0;
    }

    std::string& BodyReq() {
        return _strReq;
    }

    std::string& BodyRsp() {
        return _strRsp;
    }

protected:
    std::string _strReq;
    std::string _strRsp;

};

#endif //_TITANS_EXAMPLE_SYNCECHO_CLIENT_PLATCODEC_H_