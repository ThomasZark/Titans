#ifndef _TITANS_SPP_HANDLER_SPPHANDLER_H_
#define _TITANS_SPP_HANDLER_SPPHANDLER_H_

#include <syncincl.h>

namespace TITANS {

namespace HANDLER {

template<typename CODEC>
class SppHandler: public BaseHandler {

public:
    SppHandler(unsigned int timeout = 2000)
    :_uiTimeout(timeout) {};

    virtual ~SppHandler() {};

    virtual int HandleInput(struct stBaseHandlerContext* pContext) {

        blob_type* blob = static_cast<blob_type*>(pContext->arg1);
        return _codec.InputCheck(blob->data, blob->len, NULL);
    }

    virtual SppHandler<CODEC>* SetHandleTimeout(unsigned int timeout) {
        _uiTimeout = timeout;
        return this;
    }

    virtual unsigned int GetTimeout() {
        return _uiTimeout;
    }

protected:
    unsigned int _uiTimeout;
    CODEC _codec;
};


} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_SPP_HANDLER_SPPHANDLER_H_