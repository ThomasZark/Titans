#ifndef _TITANS_SPP_HANDLER_MT_MTCTXHANDLER_H_
#define _TITANS_SPP_HANDLER_MT_MTCTXHANDLER_H_

#include <spp/handler/SppHandler.h> 

namespace TITANS {

namespace HANDLER {

template<typename CODEC, typename MSG, typename FACTORY>
class MtCtxHandler: public SppHandler<CODEC> {

public:
    MtCtxHandler(uint32_t svr_cmd, uint32_t attr, unsigned int timeout = 2000)
    :SppHandler<CODEC>(timeout)
    ,_svrCmd(svr_cmd)
    ,_attr(attr) {};

    virtual ~MtCtxHandler() {};

    virtual int Initialize(struct stBaseHandlerContext* pContext) {

        return FACTORY::Instance()->Init(_svrCmd, _attr);
    }

    virtual int HandleProcess(struct stBaseHandlerContext* pContext) {

        blob_type* blob = static_cast<blob_type*>(pContext->arg1);
        TConnExtInfo* extinfo = static_cast<TConnExtInfo*>(blob->extdata);
        
        CServerBase* base = static_cast<CServerBase*>(pContext->arg2);
        CTCommu* commu = static_cast<CTCommu*>(blob->owner);
        
        try {
            MSG* msg = FACTORY::Instance()->CreateMsg(blob->data, blob->len);
            if(msg != NULL) {
                sockaddr_in from_addr, local_addr;
                timeval time_rcv;
                from_addr.sin_addr.s_addr = extinfo->remoteip_;
                from_addr.sin_port = extinfo->remoteport_;
                local_addr.sin_addr.s_addr = extinfo->localip_;
                local_addr.sin_port = extinfo->localport_;  
                time_rcv.tv_sec = extinfo->recvtime_;
                time_rcv.tv_usec = extinfo->tv_usec;

                msg->SetLocalAddr(local_addr);
                msg->SetFromAddr(from_addr);     
                msg->SetRcvTimestamp(time_rcv);    
                msg->SetServerBase(base);
                msg->SetTCommu(commu);
                msg->SetFlow(*(static_cast<unsigned*>(pContext->arg3)));
                msg->SetMsgTimeout(SppHandler<CODEC>::GetTimeout());
                CSyncFrame::Instance()->Process(msg);
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

#endif //_TITANS_SPP_HANDLER_MT_MTCTXHANDLER_H_