#include <syncincl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <core/plugin/PluginMgr.h>
#include <spp/server/SppServer.h>

using namespace TITANS::SERVER;
using namespace TITANS::HANDLER;
using namespace TITANS::PLUGIN;

SppServer::SppServer() {

}

SppServer::~SppServer() {

}

int SppServer::Initialize(struct stBaseSvrContext* pContext) {

    stSppSvrContext* pSppSvrContext = static_cast<stSppSvrContext*>(pContext);
    CServerBase* base = static_cast<CServerBase*>(pSppSvrContext->arg2);

    int ret = sPluginMgr::Instance()->Initialize(pSppSvrContext->arg1, pSppSvrContext->arg2);
    base->log_.LOG_P_PID(LOG_ERROR, "Plugin Init ret=%d, info=%s\n", ret, sPluginMgr::Instance()->GetLogInfo().c_str());
    return ret;
}

void SppServer::Finalize(struct stBaseSvrContext* pContext) {

    stSppSvrContext* pSppSvrContext = static_cast<stSppSvrContext*>(pContext);
    sPluginMgr::Instance()->Finalize(pSppSvrContext->arg1, pSppSvrContext->arg2);
}

int SppServer::Serve(struct stBaseSvrContext* pContext) {

    return 0;
}

int SppServer::HandleSppInitCB(void* arg1, void* arg2) {
    
    const char * etc  = (const char*)arg1;
    CServerBase* base = (CServerBase*)arg2;
    base->log_.LOG_P_PID(LOG_DEBUG, "spp_handle_init, config:%s, servertype:%d\n", etc, base->servertype());
    
    struct stSppSvrContext context = {arg1, arg2};
    int ret = Initialize(&context);
    if(0 != ret){
        base->log_.LOG_P_PID(LOG_ERROR, "init spp server error! ret=%d\n", ret);
        return ret;
    }
    return 0;
}

int SppServer::HandleSppInputCB(unsigned flow, void* arg1, void* arg2) {

    blob_type* blob = (blob_type*)arg1;
    TConnExtInfo* extinfo = (TConnExtInfo*)blob->extdata;
    CServerBase* base = (CServerBase*)arg2;

    base->log_.LOG_P(LOG_DEBUG, "spp_handle_input, %d, %d, %s, %s\n",
                     flow,
                     blob->len,
                     inet_ntoa(*(struct in_addr*)&extinfo->remoteip_),
                     format_time(extinfo->recvtime_));
   
    struct stBaseHandlerContext context = {arg1, arg2, static_cast<void*>(&flow)};
    return GetHandler()->HandleInput(&context);
}

int SppServer::HandleSppRouteCB(unsigned flow, void* arg1, void* arg2) {

    return 1;
}

int SppServer::HandleSppProcessCB(unsigned flow, void* arg1, void* arg2) {

    blob_type   * blob    = (blob_type*)arg1;
    TConnExtInfo* extinfo = (TConnExtInfo*)blob->extdata;
    CServerBase* base  = (CServerBase*)arg2;

    base->log_.LOG_P_PID(LOG_DEBUG, "spp_handle_process, %d, %d, %s, %s\n",
                         flow,
                         blob->len,
                         inet_ntoa(*(struct in_addr*)&extinfo->remoteip_),
                         format_time(extinfo->recvtime_));
    try{
        struct stBaseHandlerContext context = {arg1, arg2, static_cast<void*>(&flow)};
        int ret = GetHandler()->HandleProcess(&context);
        if(ret != 0) {
            base->log_.LOG_P_PID(LOG_ERROR, "HandleProcess failed ret=%d\n", ret);
            return ret;
        }
        return 0;
    } catch (const std::exception& e) {
        base->log_.LOG_P_PID(LOG_ERROR, "alloc error=%s\n", e.what());
    }
    return -1;
}

void SppServer::HandleSppFiniCB(void* arg1, void* arg2) {
    
    CServerBase* base = (CServerBase*)arg2;
    base->log_.LOG_P(LOG_DEBUG, "spp_handle_fini\n");

    struct stSppSvrContext context = {arg1, arg2};
    Finalize(&context);
}

char * SppServer::format_time( time_t tm) {

    static char str_tm[1024];
    struct tm tmm;
    memset(&tmm, 0, sizeof(tmm) );
    localtime_r((time_t *)&tm, &tmm);

    snprintf(str_tm, sizeof(str_tm), "[%04d-%02d-%02d %02d:%02d:%02d]",
             tmm.tm_year + 1900, tmm.tm_mon + 1, tmm.tm_mday,
             tmm.tm_hour, tmm.tm_min, tmm.tm_sec);

    return str_tm;
}
