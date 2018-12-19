#include "common/attr_define.h"
#include "common/common.h"
#include "plugin/setting.h"

#include "SppTmplSvrMsg.h"
#include "SppTmplSvr.pb.h"

#include <spp/handler/ctx/ilive_ctx/IliveMsgFactory.h>
#include <spp/plugin/uls_plugin/uls_plugin.h>
#include <spp/plugin/rpc_plugin/rpc_plugin.h>

using namespace TITANS;
using namespace TITANS::HANDLER;
using namespace TITANS::PLUGIN;

using namespace uls;
using namespace SppTmplSvr;

class RpcTestMsg: public SppTmplSvrMsg<RpcTestReq, RpcTestRsp> {

public:
    RpcTestMsg();

    virtual ~RpcTestMsg();

    virtual int Process();

protected:
    virtual int ProcessReport(int result);
};

REGIST(RPC_TEST, RpcTestMsg)

RpcTestMsg::RpcTestMsg() {
    //LogicMonitor
    SetLogicAttr(0);
    Attr_API(0, 1); //
}

RpcTestMsg::~RpcTestMsg() {

}

int RpcTestMsg::ProcessReport(int result) {
    if(result != EC_SUCC){
        Attr_API(0, 1); //
    } else {
        Attr_API(0, 1); //
    }
    ULS_LOG(_LC_INFO_, HeadReq().uid(), HeadReq().client_ip(), HeadReq().service_ip(), HeadReq().cmd(), HeadReq().subcmd())
        << "|ret=" << result << "|req=" << BodyReq().ShortDebugString();
    return EC_SUCC;
}

int RpcTestMsg::Process() {

    BodyRsp().set_retcode(GetRetcode(HeadReq().subcmd(), BodyReq().param()));
    LLOG(LOG_DEBUG, "req=%s", BodyReq().ShortDebugString().c_str());
    LLOG(LOG_DEBUG, "rsp=%s", BodyRsp().ShortDebugString().c_str());
    
    FlowTestReq req;
    FlowTestRsp rsp;
    req.set_param(BodyReq().param());
    std::string err_msg;
    int ret = Call(INS(sRpcPlugin)->Rpc(), req, rsp, err_msg);
    LLOG(LOG_DEBUG, "|%d|%s|%s|", ret, req.ShortDebugString().c_str(), rsp.ShortDebugString().c_str()); 

    return 0;
}