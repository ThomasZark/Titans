#include <unistd.h>
#include <task/RpcMgr.h>
#include <router/RouterL5.h>
#include <package/PackagerIlive.h>

#include <spp/plugin/rpc_plugin/BaseServiceRpc.h>
#include <spp/plugin/rpc_plugin/rpc_plugin.h>

#include "{{PROTO}}.pb.h"

using namespace TITANS::PLUGIN;
using namespace {{PACKAGE}};
 
class {{CAMEL_SRV_NAME}}Rpc: public RPC::BaseServiceRpc
{
public:

    virtual int Init(TASK::CRpc& rpc,
                const std::string& service_name,
                RPC::stServiceConfig_t& config) {

        string key;
        TASK::CRouterL5 router(config.l5_mid, config.l5_cid, 200);
{{#PROC_FUNC}}
        key = string(typeid({{PACKAGE}}::{{REQ_TYPE}}).name()) + typeid({{PACKAGE}}::{{RSP_TYPE}}).name();
        TASK::CPackagerIlive packager{{SUB_CMD}}({{PACKAGE}}::{{BIG_CMD}}, {{PACKAGE}}::{{SUB_CMD}});
        rpc.Regist(key, service_name, &router, &packager{{SUB_CMD}}, config.monitor, config.net, config.timeout);{{/PROC_FUNC}}
        return 0;
    }
};

REGIST_RPC({{CAMEL_SRV_NAME}}Rpc)