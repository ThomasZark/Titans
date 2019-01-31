#ifndef _TITANS_CORE_CODEC_BASECODEC_H_
#define _TITANS_CORE_CODEC_BASECODEC_H_

#include <string>

namespace TITANS {

namespace CODEC {

enum {
    CODEC_SUCC                        = 0;
    REQ_PKG_ENCODE_ERROR    = -10001;
    REQ_PKG_DECODE_ERROR    = -10002;
    RSP_PKG_ENCODE_ERROR    = -10003;
    RSP_PKG_DECODE_ERROR    = -10004;
}

class BaseCodec {

public:
    BaseCodec() {}

    virtual ~BaseCodec() {}

    //@desc - 请求包打包
    //@param
    //  strData  - 打包后数据
    //@return
    // int   - 0 成功
    virtual int ReqPkgEncode(std::string& strData) {
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
    virtual int RspPkgDecode(const char* pData, const unsigned int len, void* pExt) {
        return 0;
    }

    //@desc - 请求包合法性检查
    //@param
    //  pData   - 请求包数据
    //  len     - 请求包长度
    //  pExt    - 扩充参数
    //@return
    // int   == 0  收包未结束
    // int   < 0 失败
    // int   > 0 成功， 返回包长度
    virtual int ReqPkgCheck(const char* pData, const unsigned int len, void* pExt) {
        return len;
    }

    //@desc - 回包包合法性检查
    //@param
    //  pData   - 请求包数据
    //  len     - 请求包长度
    //  pExt    - 扩充参数
    //@return
    // int   == 0  收包未结束
    // int   < 0 失败
    // int   > 0 成功， 返回包长度
    virtual int ReqPkgCheck(const char* pData, const unsigned int len, void* pExt) {
        return len;
    }
};

} //namespace CODEC

} //namespace TITANS

#endif //_TITANS_CORE_CODEC_BASECODEC_H_