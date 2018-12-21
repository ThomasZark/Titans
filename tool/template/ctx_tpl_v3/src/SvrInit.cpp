#include <spp/server/SppServer.h>
#include <spp/server/SppServerMgr.h>

#include <spp/handler/mt/MtCtxHandler.h>
#include <spp/handler/ctx/ilive_ctx/IliveMsg.h>
#include <spp/handler/ctx/ilive_ctx/IliveMsgFactory.h>
#include <codec/ilive/IliveCodec.h>

#include "common/attr_define.h"
#include "{{PROTO}}.pb.h"

using namespace TITANS::SERVER;
using namespace TITANS::HANDLER;
using namespace TITANS::CODEC;

SppServer* GetServer() {

    static SppServer sppServerImpl;
    static MtCtxHandler<IliveCodec, IliveMsg, IliveMsgFactory> mt_ctx_handler({{PACKAGE}}::{{BIG_CMD}}, MONITOR_{{BIG_CMD}}, 2000);
    
    sppServerImpl.SetHandler(&mt_ctx_handler)->SetServerName("{{CAMEL_SRV_NAME}}");
    return &sppServerImpl;
}

REGIST_SPPSERVER(GetServer())