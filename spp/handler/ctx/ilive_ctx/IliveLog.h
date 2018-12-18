#ifndef _TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVELOG_H_
#define _TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVELOG_H_

namespace TITANS {

namespace HANDLER {

#define LLOG(lvl, fmt, args...)             \
    SF_LOG(lvl, "[%d][%d][%zu]" fmt,        \
                HeadReq().version(),        \
                HeadReq().client_type(),    \
                HeadReq().uid(),            \
                ##args);                        

#define LFLOG(lvl, fmt, args...)            \
    FLOW_LOG(lvl, "[%d][%d][%zu]" fmt,      \
                HeadReq().version(),        \
                HeadReq().client_type(),    \
                HeadReq().uid(),            \
                ##args);                                                 

} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVELOG_H_