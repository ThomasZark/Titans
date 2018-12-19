#include <spp/server/SppServer.h>
#include <spp/server/SppServerMgr.h>

#include <spp/handler/mt/MtCtxHandler.h>
#include <spp/handler/ctx/ilive_ctx/IliveMsg.h>
#include <spp/handler/ctx/ilive_ctx/IliveMsgFactory.h>
#include <codec/ilive/IliveCodec.h>

#include <SppTmplSvr.pb.h>

using namespace TITANS::SERVER;
using namespace TITANS::HANDLER;
using namespace TITANS::CODEC;

SppServer* GetServer() {

    static SppServer sppServerImpl;
    static MtCtxHandler<IliveCodec, IliveMsg, IliveMsgFactory> mt_ctx_handler(SppTmplSvr::SPP_TMPL_SVR, 0, 2000);
    
    sppServerImpl.SetHandler(&mt_ctx_handler)->SetServerName("SppTmplSvr");
    return &sppServerImpl;
}

REGIST_SPPSERVER(GetServer())