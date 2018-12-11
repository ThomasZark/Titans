#ifndef _TITANS_SPP_PLUGIN_RPC_PLUGIN_BASESERVICERPC_H_
#define _TITANS_SPP_PLUGIN_RPC_PLUGIN_BASESERVICERPC_H_

#include <core/common/Singleton.h>
#include <task/RpcMgr.h>

namespace RPC {

struct stServiceConfig_t {

    std::string service;
    uint32_t l5_mid;
    uint32_t l5_cid;
    uint32_t timeout;
    uint32_t net;
};

class BaseServiceRpc {

public:
    BaseServiceRpc() {}
    
    virtual ~BaseServiceRpc() {}

    virtual int Init(TASK::CRpc& rpc,
                    const std::string& service_name,
                    struct stServiceConfig_t& config) {
        return 0;
    }
};

} //namespace RPC

#define RPC_CAT(file, line) file##line
#define REGIST_RPC(NAME)    \
static int RPC_CAT(R, __LINE__) = TITANS::PLUGIN::sRpcPlugin::Instance()->Regist(#NAME, TITANS::Singleton<NAME>::Instance());

#endif //_TITANS_SPP_PLUGIN_RPC_PLUGIN_BASESERVICERPC_H_