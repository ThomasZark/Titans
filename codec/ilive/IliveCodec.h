#ifndef _TITANS_CORE_CODEC_ILIVE_ILIVECODEC_H_
#define _TITANS_CORE_CODEC_ILIVE_ILIVECODEC_H_

#include <core/codec/BaseCodec.h>

namespace TITANS {

namespace CODEC {

class IliveCodec : public BaseCodec {

public:
    IliveCodec() {}

    virtual ~IliveCodec() {}

    virtual int Encode(std::string& strData) {
        return 0;
    }

    virtual int Decode(const char* pData, const unsigned int len, void* pExt) {
        return 0;
    }   

    virtual int InputCheck(const char* pData, const unsigned int len, void* pExt) {
        return 0;
    }

};

} //namespace CODEC

} //namespace TITANS

#endif //_TITANS_CORE_CODEC_ILIVE_ILIVECODEC_H_