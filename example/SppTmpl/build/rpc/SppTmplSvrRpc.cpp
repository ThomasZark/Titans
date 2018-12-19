#include <unistd.h>
#include <task/RpcMgr.h>
#include <router/RouterL5.h>
#include <package/PackagerIlive.h>

#include <spp/plugin/rpc_plugin/BaseServiceRpc.h>
#include <spp/plugin/rpc_plugin/rpc_plugin.h>

#include "SppTmplSvr.pb.h"

using namespace TITANS::PLUGIN;
using namespace SppTmplSvr;

class SppTmplSvrRpc: public RPC::BaseServiceRpc
{
public:

    virtual int Init(TASK::CRpc& rpc,
                const std::string& service_name,
                RPC::stServiceConfig_t& config) {

        string key;
        TASK::CRouterL5 router(config.l5_mid, config.l5_cid, 200);

        key = string(typeid(FlowTestReq).name()) + typeid(FlowTestRsp).name();
        TASK::CPackagerIlive packagerFLOW_TEST(SPP_TMPL_SVR, FLOW_TEST);
        rpc.Regist(key, service_name, &router, &packagerFLOW_TEST, config.monitor, config.net, config.timeout);

        return 0;
    }
};

REGIST_RPC(SppTmplSvrRpc)