#ifndef _TITANS_CORE_CODEC_BASECODEC_H_
#define _TITANS_CORE_CODEC_BASECODEC_H_

#include <string>

namespace TITANS {

namespace CODEC {

class BaseCodec {

public:
    BaseCodec() {}

    virtual ~BaseCodec() {}

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

#endif //_TITANS_CORE_CODEC_BASECODEC_H_