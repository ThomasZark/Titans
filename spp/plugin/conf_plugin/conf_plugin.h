#ifndef _TITANS_SPP_PLUGIN_CONF_PLUGIN_CONF_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_CONF_PLUGIN_CONF_PLUGIN_H_

#include <core/common/Singleton.h>
#include <spp/plugin/TmplPlugin.h>

namespace TITANS {

namespace PLUGIN {

class ConfPlugin:public TmplPlugin {

public:
    ConfPlugin();

    virtual ~ConfPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    virtual bool LoadParamConfig(const libconfig::Setting& sSetting);

    //@desc - 业务配置读取
    //@param
    //@return
    virtual bool LoadConf();

protected:
    std::string strConfPath;
};
  
typedef Singleton<ConfPlugin> sConfPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_CONF_PLUGIN_CONF_PLUGIN_H_