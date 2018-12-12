#ifndef _TITANS_SPP_PLUGIN_ULS_PLUGIN_ULS_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_ULS_PLUGIN_ULS_PLUGIN_H_

#include <core/common/Singleton.h>
#include <spp/plugin/TmplPlugin.h>
#include <uls_log_api2.h>

namespace TITANS {

namespace PLUGIN {

class ULSPlugin:public TmplPlugin {

public:
    ULSPlugin();

    virtual ~ULSPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    virtual bool LoadParamConfig(const libconfig::Setting& sSetting);

protected:
    std::string strUlsPath;
    int iUlsId;
    int iWaterLogId;
    int iColorLogId;
    int iWaterLogFlag;
    int iColorLogFlag;
    bool bOpenLocalLog;
};
  
typedef Singleton<ULSPlugin> sUlsPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_ULS_PLUGIN_ULS_PLUGIN_H_