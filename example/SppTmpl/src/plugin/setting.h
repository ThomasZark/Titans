#ifndef _PLUGIN_SETTING_H_
#define _PLUGIN_SETTING_H_

#include <core/common/Singleton.h>
#include <spp/plugin/conf_plugin/conf_plugin.h>

class SettingPlugin: public TITANS::PLUGIN::ConfPlugin {

public:
    SettingPlugin();

    virtual ~SettingPlugin();

    virtual bool LoadConf();

public:
    //@auto setting
	uint32_t uiMid;
	uint32_t uiCid;
	uint32_t uiBid;
};

typedef TITANS::Singleton<SettingPlugin> sSettingPlugin;

#endif //_PLUGIN_SETTING_H_