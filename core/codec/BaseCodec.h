#ifndef _TITANS_CORE_CODEC_BASECODEC_H_
#define _TITANS_CORE_CODEC_BASECODEC_H_

#include <string>

namespace TITANS {

namespace CODEC {

class BaseCodec {

public:
    BaseCodec() {}

    virtual ~BaseCodec() {}

    //@desc - 回包打包
    //@param
    //  strData  - 打包后数据
    //@return
    // int   - 0 成功
    virtual int RspPkgEncode(std::string& strData) {
        return 0;
    }

    //@desc - 请求包解包
    //@param
    //  pData   - 请求包数据
    //  len     - 请求包长度
    //  pExt    - 扩充参数
    //@return
    // int   - 0 成功
    virtual int ReqPkgDecode(const char* pData, const unsigned int len, void* pExt) {
        return 0;
    }

    //@desc - 请求包合法性检查
    //@param
    //  pData   - 请求包数据
    //  len     - 请求包长度
    //  pExt    - 扩充参数
    //@return
    // int   - 0 成功
    virtual int ReqPkgCheck(const char* pData, const unsigned int len, void* pExt) {
        return 0;
    }
};

} //namespace CODEC

} //namespace TITANS

#endif //_TITANS_CORE_CODEC_BASECODEC_H_