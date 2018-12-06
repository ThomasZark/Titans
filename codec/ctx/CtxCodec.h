#ifndef _TITANS_CODEC_CTX_CTXCODEC_H_
#define _TITANS_CODEC_CTX_CTXCODEC_H_

#include <proto/proto_ilive.h>

namespace TITANS {

namespace CODEC {

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
    ENCODE_ITEM(client_type);
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
    ENCODE_ITEM(buss_id);
    ENCODE_ITEM(target_env);
    ENCODE_ITEM(client_id);
    ENCODE_ITEM(client_ipv6);
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


} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_CODEC_CTX_CTXCODEC_H_