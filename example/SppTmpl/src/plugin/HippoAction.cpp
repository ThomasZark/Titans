#include <spp/plugin/hippo_consumer_plugin/hippo_consumer_plugin.h>
#include <proto/proto_ilive.h>
#include <SppTmplSvr.pb.h>

using namespace TITANS::PLUGIN;
using namespace SppTmplSvr;
using namespace proto;

ERspHandleStatus HandleHippo(const string& topic, const string& input, string& output, CServerBase* base) {

    HippoComsumerTestReq req;
    req.set_param(input);
    
    std::string reqBody;
    req.SerializeToString(&reqBody);

    proto::ILiveReqPkg iLiveReqPkg;
    iLiveReqPkg.package().set_cmd(SPP_TMPL_SVR);
    iLiveReqPkg.package().set_subcmd(HIPPO_COMSUMER_TEST);
    iLiveReqPkg.package().set_ex(reqBody);

    const int ret = iLiveReqPkg.Encode();
    if (ret != 0) {
        base->log_.LOG_P_PID(LOG_ERROR, "[encode_SyncNowFeedsReq] req=[%s]", req.ShortDebugString().c_str());
        return NOT_HANDLE;
    }
    output = iLiveReqPkg.data();
    base->log_.LOG_P_PID(LOG_ERROR, "output size:%u\n", output.size());
    return NOT_NEED_RSP;
}

REGIST_HIPPO(HandleHippo)