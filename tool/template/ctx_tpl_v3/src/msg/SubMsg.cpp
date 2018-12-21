#include "common/attr_define.h"
#include "common/common.h"
#include "plugin/Setting.h"

#include "{{CAMEL_SRV_NAME}}Msg.h"
#include "{{PROTO}}.pb.h"

#include <spp/handler/ctx/ilive_ctx/IliveMsgFactory.h>
#include <spp/plugin/uls_plugin/uls_plugin.h>
#include <spp/plugin/rpc_plugin/rpc_plugin.h>

using namespace TITANS;
using namespace TITANS::HANDLER;
using namespace TITANS::PLUGIN;

using namespace uls;
using namespace {{PACKAGE}};
{{#PROC_FUNC}}
class {{FUNC_NAME}}Msg: public {{CAMEL_SRV_NAME}}Msg<{{REQ_TYPE}}, {{RSP_TYPE}}> {

public:
    {{FUNC_NAME}}Msg();

    virtual ~{{FUNC_NAME}}Msg();

    virtual int Process();

protected:
    virtual int ProcessReport(int result);
};

REGIST({{SUB_CMD}}, {{FUNC_NAME}}Msg)

{{FUNC_NAME}}Msg::{{FUNC_NAME}}Msg() {
    //LogicMonitor
    SetLogicAttr(MONITOR_{{SUB_CMD}});
    Attr_API(MONITOR_{{SUB_CMD}}_ALL, 1); //{{SUB_CMD}}总请求
}

{{FUNC_NAME}}Msg::~{{FUNC_NAME}}Msg() {

}

int {{FUNC_NAME}}Msg::ProcessReport(int result) {
    if(result != EC_SUCC){
        Attr_API(MONITOR_{{SUB_CMD}}_FAILED, 1); //{{SUB_CMD}}总失败
    } else {
        Attr_API(MONITOR_{{SUB_CMD}}_SUCC, 1); //{{SUB_CMD}}总成功
    }
    ULS_LOG(_LC_INFO_, HeadReq().uid(), HeadReq().client_ip(), HeadReq().service_ip(), {{BIG_CMD}}, HeadReq().subcmd())
        << "|ret=" << result << "|req=" << BodyReq().ShortDebugString();
    return EC_SUCC;
}

int {{FUNC_NAME}}Msg::Process() {

    return EC_SUCC;
}
{{/PROC_FUNC}}