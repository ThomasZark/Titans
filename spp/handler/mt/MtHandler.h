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
        CServerBase* base = static_cast<CServerBase*>(pContext->arg2);
        CTCommu* commu = static_cast<CTCommu*>(blob->owner);
        
        try {
            MSG* msg = new MSG();
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