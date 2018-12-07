#ifndef _TITANS_SPP_HANDLER_SYNC_SYNCCTXHANDLER_H_
#define _TITANS_SPP_HANDLER_SYNC_SYNCCTXHANDLER_H_

#include <memory>
#include <spp/handler/SppHandler.h> 

namespace TITANS {

namespace HANDLER {

template<typename CODEC, typename MSG, typename FACTORY>
class SyncCtxHandler: public SppHandler<CODEC> {

public:
    SyncCtxHandler(uint32_t svr_cmd, uint32_t attr, unsigned int timeout = 2000)
    :SppHandler<CODEC>(timeout)
    ,_svrCmd(svr_cmd)
    ,_attr(attr) {};

    virtual ~SyncCtxHandler() {};

    virtual int Initialize(struct stBaseHandlerContext* pContext) {

        return FACTORY::Instance()->Init(_svrCmd, _attr);
    }

    virtual int HandleProcess(struct stBaseHandlerContext* pContext) {

        blob_type* blob = static_cast<blob_type*>(pContext->arg1);
        CServerBase* base = static_cast<CServerBase*>(pContext->arg2);
        CTCommu* commu = static_cast<CTCommu*>(blob->owner);
        
        try {
            std::shared_ptr<MSG> msg(FACTORY::Instance()->CreateMsg(blob->data, blob->len));
            if(msg != NULL) {
                msg->SetServerBase(base);
                msg->SetTCommu(commu);
                msg->SetFlow(*(static_cast<unsigned*>(pContext->arg3)));
                msg->SetMsgTimeout(SppHandler<CODEC>::GetTimeout());
                msg->HandleProcess();
                return 0;
            }
            base->log_.LOG_P_PID(LOG_ERROR, "CreateMsg error\n");
        } catch (const std::exception& e) {
            base->log_.LOG_P_PID(LOG_ERROR, "alloc error=%s\n", e.what());
        }
        return -1;
    }

protected:
    uint32_t _svrCmd;
    uint32_t _attr;
};


} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_SPP_HANDLER_SYNC_SYNCCTXHANDLER_H_