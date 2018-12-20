#include "common/attr_define.h"
#include "common/common.h"
#include "plugin/Setting.h"

#include "SppTmplSvrMsg.h"
#include "SppTmplSvr.pb.h"

#include <spp/handler/ctx/ilive_ctx/IliveMsgFactory.h>
#include <spp/plugin/uls_plugin/uls_plugin.h>
#include <spp/plugin/rpc_plugin/rpc_plugin.h>
#include <spp/plugin/tc_plugin/tc_plugin.h>

using namespace TITANS;
using namespace TITANS::HANDLER;
using namespace TITANS::PLUGIN;

using namespace uls;
using namespace SppTmplSvr;

class TcTestMsg: public SppTmplSvrMsg<TcTestReq, TcTestRsp> {

public:
    TcTestMsg();

    virtual ~TcTestMsg();

    virtual int Process();

protected:
    virtual int ProcessReport(int result);
};

REGIST(TC_TEST, TcTestMsg)

TcTestMsg::TcTestMsg() {
    //LogicMonitor
    SetLogicAttr(0);
    SetBodyCodec(&_bodyCodec);
    Attr_API(0, 1); //
}

TcTestMsg::~TcTestMsg() {

}

int TcTestMsg::ProcessReport(int result) {
    if(result != EC_SUCC){
        Attr_API(0, 1); //
    } else {
        Attr_API(0, 1); //
    }
    ULS_LOG(_LC_INFO_, HeadReq().uid(), HeadReq().client_ip(), HeadReq().service_ip(), HeadReq().cmd(), HeadReq().subcmd())
        << "|ret=" << result << "|req=" << BodyReq().ShortDebugString();
    return EC_SUCC;
}

int TcTestMsg::Process() {

    BodyRsp().set_retcode(GetRetcode(HeadReq().subcmd(), BodyReq().param()));
    LLOG(LOG_DEBUG, "req=%s", BodyReq().ShortDebugString().c_str());
    LLOG(LOG_DEBUG, "rsp=%s", BodyRsp().ShortDebugString().c_str());

    stringstream ss;
    ss << "module=conversion_push&action=gift_push&obj1=" <<BodyRsp().retcode()
        << "&uin="<<HeadReq().uid();

    INS(sTCPlugin)->ReportTDBank(ss.str());
    return 0;
}