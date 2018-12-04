#ifndef _TITANS_CORE_SERVER_SPP_SPPSERVERMGR_H_
#define _TITANS_CORE_SERVER_SPP_SPPSERVERMGR_H_

#include <core/common/Singleton.h>
#include <core/serrver/spp/SppServer.h>

namespace TITANS {

namespace SERVER {

class SppServerMgr {

public:
    SppServerMgr() {}
    
    virtual ~SppServerMgr() {}

    SppServerMgr* SetSppServerImpl(SppServer* pSppServerImpl) {
        _pSppServerImpl = ptr;
        return this;
    }

    SppServer* GetSppServerImpl() {
        return _pSppServerImpl;
    }

protected:
    SppServer* _pSppServerImpl;
};

typedef Singleton<SppServerMgr> sSppServerMgr;

}//namespace SERVER

}//namespace TITANS

#endif //_TITANS_CORE_SERVER_SPP_SPPSERVERMGR_H_