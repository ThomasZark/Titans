#ifndef _TITANS_SPP_HANDLER_MT_MTHANDLER_H_
#define _TITANS_SPP_HANDLER_MT_MTHANDLER_H_

#include <spp/handler/SppHandler.h> 

namespace TITANS {

namespace HANDLER {

template<typename CODEC, typename MSG>
class MtHandler: public SppHandler<CODEC> {

public:
    MtHandler(unsigned int timeout = 2000)
    :SppHandler<CODEC>(timeout) {};

    virtual ~MtHandler() {};

    virtual int HandleProcess(struct stBaseHandlerContext* pContext) {

        blob_type* blob = static_cast<blob_type*>(pContext->arg1);
        TConnExtInfo* extinfo = static_cast<TConnExtInfo*>(blob->extdata);

        CServerBase* base = static_cast<CServerBase*>(pContext->arg2);
        CTCommu* commu = static_cast<CTCommu*>(blob->owner);
        
        try {
            MSG* msg = new MSG();
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
            msg->SetReqPkg(blob->data, blob->len); /* 微线程有独立空间,这里要拷贝一次报文 */
            CSyncFrame::Instance()->Process(msg);
            return 0;
        } catch (const std::exception& e) {
            base->log_.LOG_P_PID(LOG_ERROR, "alloc error=%s\n", e.what());
        }
        return -1;
    }
};


} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_SPP_HANDLER_MT_MTHANDLER_H_