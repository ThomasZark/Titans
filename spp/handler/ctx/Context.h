#ifndef _TITANS_CORE_HANDLER_CTX_CONTEXT_H_
#define _TITANS_CORE_HANDLER_CTX_CONTEXT_H_

#include <memory>
#include <string>

namespace TITANS {

namespace HANDLER {

#define ENCODE_ITEM(item) \
    if(!dst->has_##item() && src->has_##item()) { \
        dst->set_##item(src->item()); \
    } \

//@desc - 请求包头填充，不会覆盖dst原有的数据，oidb其他协议需要重载该函数
//@param
//  dst     - 目标包头
//  src     - 源包头
//@return
// void
inline void EncodeHead(iLiveRequest* dst, iLiveRequest* src) {

    ENCODE_ITEM(version);
    ENCODE_ITEM(seq);
    ENCODE_ITEM(client_ip);
    ENCODE_ITEM(uid);
    ENCODE_ITEM(client_ip);
    ENCODE_ITEM(service_ip);
    ENCODE_ITEM(business);
    ENCODE_ITEM(auth_key);
    ENCODE_ITEM(auth_type);
    ENCODE_ITEM(auth_ip);
    ENCODE_ITEM(auth_appid);
    ENCODE_ITEM(tinyid);
    ENCODE_ITEM(client_port);
    ENCODE_ITEM(original_id);
    ENCODE_ITEM(original_key);
    ENCODE_ITEM(original_key_type);
    ENCODE_ITEM(original_id_type);
    ENCODE_ITEM(original_auth_appid);
}

//@desc - 回包包头填充，不会覆盖dst原有的数据，oidb其他协议需要重载该函数
//@param
//  dst     - 目标包头
//  src     - 源包头
//@return
// void
inline void EncodeHead(iLiveResponse* dst, iLiveRequest* src) {

    ENCODE_ITEM(version);
    ENCODE_ITEM(cmd);
    ENCODE_ITEM(subcmd);
    ENCODE_ITEM(seq);
    ENCODE_ITEM(uid);
    ENCODE_ITEM(client_type);
}

template<typename HEADREQ, typename HEADRSP>
class Context {

public:
    Context() {}
    virtual ~Context() {}

    //@desc - 设置请求包数据
    //@param
    //  data     - 请求包数据
    //  size     - 请求包长度
    //@return
    // void
    void SetReqPkg(const char* data, uint32_t size) {
        m_req_pkg.set_data(data, size);
    }

    //@desc - 获取回包数据
    //@param
    //@return
    // string   - 回包数据
    std::string GetRspPkg() {
        return m_rsp_pkg.data();
    }

    //@desc - 请求包解包
    //@param
    //@return
    // int   - 0 成功
    int ReqPkgDecode() {
        return m_req_pkg.Decode();
    }

    //@desc - 回包打包
    //@param
    //@return
    // int   - 0 成功
    int RspPkgEncode() {
        return m_rsp_pkg.Encode();
    }

    //@desc - 获取请求包头
    //@param
    //@return
    // iLiveRequest&   - 请求包头
    iLiveRequest& HeadReq() {
        return m_req_pkg.package();
    }

    //@desc - 获取回包包头
    //@param
    //@return
    // iLiveResponse&   - 回包包头
    iLiveResponse& HeadRsp() {
        return m_rsp_pkg.package();
    }

private:
    proto::ILiveReqPkg m_req_pkg;
    proto::ILiveRespPkg m_rsp_pkg;

};

typedef std::shared_ptr<IliveContext> IliveCtxPtr;

} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_CORE_HANDLER_CTX_CONTEXT_H_