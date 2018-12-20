#include "common/attr_define.h"
#include "common/common.h"
#include "plugin/Setting.h"

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

class HippoComsumerTestMsg: public SppTmplSvrMsg<HippoComsumerTestReq, HippoComsumerTestRsp> {

public:
    HippoComsumerTestMsg();

    virtual ~HippoComsumerTestMsg();

    virtual int Process();

protected:
    virtual int ProcessReport(int result);
};

REGIST(HIPPO_COMSUMER_TEST, HippoComsumerTestMsg)

HippoComsumerTestMsg::HippoComsumerTestMsg() {
    //LogicMonitor
    SetLogicAttr(0);
    SetBodyCodec(&_bodyCodec);
    Attr_API(0, 1); //
}

HippoComsumerTestMsg::~HippoComsumerTestMsg() {

}

int HippoComsumerTestMsg::ProcessReport(int result) {
    if(result != EC_SUCC){
        Attr_API(0, 1); //
    } else {
        Attr_API(0, 1); //
    }
    ULS_LOG(_LC_INFO_, HeadReq().uid(), HeadReq().client_ip(), HeadReq().service_ip(), HeadReq().cmd(), HeadReq().subcmd())
        << "|ret=" << result << "|req=" << BodyReq().ShortDebugString();
    return EC_SUCC;
}

int HippoComsumerTestMsg::Process() {

    BodyRsp().set_retcode(GetRetcode(HeadReq().subcmd(), 0));
    LLOG(LOG_DEBUG, "req=%s", BodyReq().ShortDebugString().c_str());
    LLOG(LOG_DEBUG, "rsp=%s", BodyRsp().ShortDebugString().c_str());
    return 0;
}