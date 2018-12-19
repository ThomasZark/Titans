#include <core/plugin/PluginMgr.h>

#include "setting.h"

SettingPlugin::SettingPlugin() {

}

SettingPlugin::~SettingPlugin() {

}

#define LOOK_UP_VALUE(value) \
{\
    if(!m_oCfg.lookupValue(#value, value)){\
        _ssLog<< "[SettingConfig]read "#value" failed." << std::endl;\
        return false;\
    }\
    _ssLog<< "[SettingConfig]"#value << "=" << value << std::endl;\
}

bool SettingPlugin::LoadConf() {
    try {
        libconfig::Config m_oCfg;
        m_oCfg.readFile(strConfPath.c_str());
        
        //@auto setting
		LOOK_UP_VALUE(uiMid);
		LOOK_UP_VALUE(uiCid);
		LOOK_UP_VALUE(uiBid);
        
    } catch (const libconfig::ParseException &e) {
        _ssLog<< "[SettingConfig]parse failed, err=" <<e.what() <<std::endl;
        return false;
    } catch (const libconfig::SettingException &e) {
        _ssLog<< "[SettingConfig]setting error=" << e.what() <<std::endl;
        return false;
    } catch (const std::exception &e){
        _ssLog<< "[SettingConfig]error=" << e.what() <<std::endl;
        return false;
    }
    return true;
}

REGIST_PLUGIN(SettingPlugin)