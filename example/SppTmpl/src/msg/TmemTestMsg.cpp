#include "common/attr_define.h"
#include "common/common.h"
#include "plugin/Setting.h"

#include "SppTmplSvrMsg.h"
#include "SppTmplSvr.pb.h"

#include <spp/handler/ctx/ilive_ctx/IliveMsgFactory.h>
#include <spp/plugin/uls_plugin/uls_plugin.h>
#include <spp/plugin/rpc_plugin/rpc_plugin.h>
#include <spp/plugin/tmem_plugin/tmem_plugin.h>

using namespace TITANS;
using namespace TITANS::HANDLER;
using namespace TITANS::PLUGIN;

using namespace uls;
using namespace SppTmplSvr;

class TmemTestMsg: public SppTmplSvrMsg<TmemTestReq, TmemTestRsp> {

public:
    TmemTestMsg();

    virtual ~TmemTestMsg();

    virtual int Process();

protected:
    virtual int ProcessReport(int result);
};

REGIST(TMEM_TEST, TmemTestMsg)

TmemTestMsg::TmemTestMsg() {
    //LogicMonitor
    SetLogicAttr(0);
    SetBodyCodec(&_bodyCodec);
    Attr_API(0, 1); //
}

TmemTestMsg::~TmemTestMsg() {

}

int TmemTestMsg::ProcessReport(int result) {
    if(result != EC_SUCC){
        Attr_API(0, 1); //
    } else {
        Attr_API(0, 1); //
    }
    ULS_LOG(_LC_INFO_, HeadReq().uid(), HeadReq().client_ip(), HeadReq().service_ip(), HeadReq().cmd(), HeadReq().subcmd())
        << "|ret=" << result << "|req=" << BodyReq().ShortDebugString();
    return EC_SUCC;
}

int TmemTestMsg::Process() {

    BodyRsp().set_retcode(GetRetcode(HeadReq().subcmd(), BodyReq().param()));
    LLOG(LOG_DEBUG, "req=%s", BodyReq().ShortDebugString().c_str());
    LLOG(LOG_DEBUG, "rsp=%s", BodyRsp().ShortDebugString().c_str());

    MtTmemClient mt_client;
    mt_client.Init(INS(sSettingPlugin)->uiBid, INS(sSettingPlugin)->uiMid, INS(sSettingPlugin)->uiCid);

    TmemData tmem_data;
    TmemData tmem_data_v2;
    TmemData tmem_data_v3;
    std::string key = "tmem_test";
    int cas = -1;
    tmem_data.set_test(BodyRsp().retcode());
    int ret    = INS(sTmemPlugin)->SetTmemData(&mt_client, key,  tmem_data, &cas);
    int ret_v2 = INS(sTmemPlugin)->GetTmemData(&mt_client, key, &tmem_data_v2, &cas);
    int ret_v3 = INS(sTmemPlugin)->DelTmemData(&mt_client, key);
    int ret_v4 = INS(sTmemPlugin)->GetTmemData(&mt_client, key, &tmem_data_v3, &cas);
    
    LLOG(LOG_DEBUG, "|%d|%d|%d|%d|%s|%s|%s|", ret, ret_v2, ret_v3, ret_v4, 
                                            tmem_data.DebugString().c_str(),
                                            tmem_data_v2.DebugString().c_str(),
                                            tmem_data_v3.DebugString().c_str());
    
    return 0;
}