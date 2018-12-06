#ifndef _TITANS_CORE_SERVER_SPPSERVERMGR_H_
#define _TITANS_CORE_SERVER_SPPSERVERMGR_H_

#include <core/common/Singleton.h>
#include <spp/server/SppServer.h>

namespace TITANS {

namespace SERVER {

class SppServerMgr {

public:
    SppServerMgr() {}
    
    virtual ~SppServerMgr() {}

    SppServerMgr* SetSppServerImpl(SppServer* pSppServerImpl) {
        _pSppServerImpl = pSppServerImpl;
        return this;
    }

    SppServer* GetSppServerImpl() {
        return _pSppServerImpl;
    }

protected:
    SppServer* _pSppServerImpl;
};

typedef Singleton<SppServerMgr> sSppServerMgr;

#define CAT(file, line) file##line
#define REGIST_SPPSERVER(server) \
static SppServerMgr* CAT(RS, __LINE__) = TITANS::SERVER::sSppServerMgr::Instance()->SetSppServerImpl(server);

}//namespace SERVER

}//namespace TITANS

#endif //_TITANS_CORE_SERVER_SPPSERVERMGR_H_