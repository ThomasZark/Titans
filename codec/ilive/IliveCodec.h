#ifndef _TITANS_CORE_CODEC_ILIVE_ILIVECODEC_H_
#define _TITANS_CORE_CODEC_ILIVE_ILIVECODEC_H_

#include <proto/proto_ilive.h>
#include <core/codec/BaseCodec.h>

namespace TITANS {

namespace CODEC {

class IliveCodec : public BaseCodec {

public:
    IliveCodec() {}

    virtual ~IliveCodec() {}

    virtual int RspPkgEncode(std::string& strData) {
        int ret = _rspPkg.Encode();
        strData = _rspPkg.data();
        return ret;
    }

    virtual int ReqPkgDecode(const char* pData, const unsigned int len, void* pExt) {
        
        _reqPkg.set_data(pData, len);
        return _reqPkg.Decode();
    }   

    virtual int ReqPkgCheck(const char* pData, const unsigned int len, void* pExt) {
        
        return proto::ILiveCheck(const_cast<char*>(pData), len);
    }

    //@desc - 获取请求包头
    //@param
    //@return
    // iLiveRequest&   - 请求包头
    iLiveRequest& HeadReq() {
        return _reqPkg.package();
    }

    //@desc - 获取回包包头
    //@param
    //@return
    // iLiveResponse&   - 回包包头:
    iLiveResponse& HeadRsp() {
        return _rspPkg.package();
    }

protected:
    proto::ILiveReqPkg _reqPkg;
    proto::ILiveRespPkg _rspPkg;

};

} //namespace CODEC

} //namespace TITANS

#endif //_TITANS_CORE_CODEC_ILIVE_ILIVECODEC_H_