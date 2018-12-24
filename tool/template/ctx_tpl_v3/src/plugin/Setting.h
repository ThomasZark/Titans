#ifndef {{UP_SRV_NAME}}_SRC_PLUGIN_SETTING_H
#define {{UP_SRV_NAME}}_SRC_PLUGIN_SETTING_H

#include <core/common/Singleton.h>
#include <spp/plugin/conf_plugin/conf_plugin.h>

class SettingPlugin: public TITANS::PLUGIN::ConfPlugin {

public:
    SettingPlugin();

    virtual ~SettingPlugin();

    virtual bool LoadConf();

public:
    //@auto setting
    
};

typedef TITANS::Singleton<SettingPlugin> sSettingPlugin;

#endif //{{UP_SRV_NAME}}_SRC_PLUGIN_SETTING_H
