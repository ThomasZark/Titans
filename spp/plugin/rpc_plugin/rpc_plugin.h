#ifndef _TITANS_SPP_PLUGIN_RPC_PLUGIN_RPC_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_RPC_PLUGIN_RPC_PLUGIN_H_

#include <core/common/Singleton.h>
#include <spp/plugin/TmplPlugin.h>
#include <spp/plugin/rpc_plugin/BaseServiceRpc.h>

namespace TITANS {

namespace PLUGIN {

class RpcPlugin:public TmplPlugin {

public:
    RpcPlugin();

    virtual ~RpcPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    virtual bool LoadParamConfig(const libconfig::Setting& sSetting);

    int Regist(const char* pName, RPC::BaseServiceRpc* pService);

    TASK::CRpc& Rpc();

protected:

    bool LoadServiceConfig(const char* etc);

    std::map<std::string, RPC::BaseServiceRpc*> m_service;
    std::vector<RPC::stServiceConfig_t> m_serviceConfigure;
    std::string m_strConfigPath;
    TASK::CRpc m_Rpc;
};

typedef Singleton<RpcPlugin> sRpcPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_RPC_PLUGIN_RPC_PLUGIN_H_