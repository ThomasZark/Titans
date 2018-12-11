#ifndef _TITANS_SPP_PLUGIN_ULS_PLUGIN_ULS_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_ULS_PLUGIN_ULS_PLUGIN_H_

#include <core/common/Singleton.h>
#include <core/plugin/Plugin.h>
#include <uls_log_api2.h>

namespace TITANS {

namespace PLUGIN {

class ULSPlugin:public Plugin {

public:
    ULSPlugin();

    virtual ~ULSPlugin();

    virtual int Initialize(void* arg1, void* arg2);

};
  
typedef Singleton<ULSPlugin> sUlsPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_ULS_PLUGIN_ULS_PLUGIN_H_