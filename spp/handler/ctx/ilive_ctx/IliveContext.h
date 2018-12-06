#ifndef _TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVECONTEXT_H_
#define _TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVECONTEXT_H_

#include <memory>
#include <codec/ilive/IliveCodec.h>

namespace TITANS {

namespace HANDLER {

class IliveContext {

public:
    IliveContext() {}
    virtual ~IliveContext() {}

    //@desc - 请求包解包
    //@param
    //@return
    // int   - 0 成功
    int ReqPkgDecode(const char* pData, const unsigned int len, void* pExt) {
        return _codec.ReqPkgDecode(pData, len, pExt);
    }

    //@desc - 回包打包
    //@param
    //@return
    // int   - 0 成功
    int RspPkgEncode(std::string& strData) {
        return _codec.RspPkgEncode(strData);
    }

    //@desc - 获取请求包头
    //@param
    //@return
    // iLiveRequest&   - 请求包头
    iLiveRequest& HeadReq() {
        return _codec.HeadReq();
    }

    //@desc - 获取回包包头
    //@param
    //@return
    // iLiveResponse&   - 回包包头:
    iLiveResponse& HeadRsp() {
        return _codec.HeadRsp();
    }

    //@desc - 获取ex数据
    //@param
    //@return
    // void*   - ex数据:
    void* GetEx() {
        return m_pEx;
    }

    //@desc - 设置ex数据
    //@param
    //  ex  -ex数据
    //@return
    void SetEx(void* ex) {
        m_pEx = ex;
    }

private:
    TITANS::CODEC::IliveCodec _codec;
    void* m_pEx;
};

typedef std::shared_ptr<IliveContext> IliveCtxPtr;

} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVECONTEXT_H_
