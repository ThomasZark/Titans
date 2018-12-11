#ifndef _TITANS_SPP_PLUGIN_RPC_PLUGIN_RPC_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_RPC_PLUGIN_RPC_PLUGIN_H_

#include <core/common/Singleton.h>
#include <core/plugin/Plugin.h>
#include <spp/plugin/rpc_plugin/BaseServiceRpc.h>

namespace TITANS {

namespace PLUGIN {

class RpcPlugin:public Plugin {

public:
    RpcPlugin();

    virtual ~RpcPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    int Regist(const char* pName, RPC::BaseServiceRpc* pService);

    TASK::CRpc& Rpc();

protected:

    bool LoadConfig(const char* etc, CServerBase* base);

    std::map<std::string, RPC::BaseServiceRpc*> m_service;
    std::vector<RPC::stServiceConfig_t> m_serviceConfigure;
    TASK::CRpc m_Rpc;
};

typedef Singleton<RpcPlugin> sRpcPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_RPC_PLUGIN_RPC_PLUGIN_H_