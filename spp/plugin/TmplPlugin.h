#ifndef _TITANS_SPP_PLUGIN_CONFPLUGIN_H_
#define _TITANS_SPP_PLUGIN_CONFPLUGIN_H_

#include <libconfig.h++>
#include <sstream>

#include <core/plugin/Plugin.h>

namespace TITANS {

namespace PLUGIN {

class TmplPlugin: public Plugin {

public:
    TmplPlugin() {};

    virtual ~TmplPlugin() {};

    //@desc - 插件参数初设化
    //@param
    //@return
    virtual bool LoadParamConfig(const libconfig::Setting & sSetting) {
        return true;
    }

protected:
    
    bool LoadConfig(const char* etc) {
        try {
            libconfig::Config m_oCfg;
            m_oCfg.readFile(etc);
            const libconfig::Setting &plugin_array = m_oCfg.lookup("plugin_mgr");
            for(int index = 0; index < plugin_array.getLength(); index ++) {
                std::string name;
                if(plugin_array[index].lookupValue("name", name)) {
                    if(name == GetPluginName()) {
                        return LoadParamConfig(plugin_array[index]);
                    }
                }
            }
            _ssLog<< "[" << GetPluginName() << "]param not found" << std::endl;
            return false;
        } catch (const libconfig::ParseException &e) {
            _ssLog<< "[" << GetPluginName() << "]parse failed, err=" <<e.what() <<std::endl;
            return false;
        } catch (const libconfig::SettingException &e) {
            _ssLog<< "[" << GetPluginName() << "]setting error=" << e.what() <<std::endl;
            return false;
        } catch (const std::exception &e){
            _ssLog<< "[" << GetPluginName() << "]error=" << e.what() <<std::endl;
            return false;
        }
        return true;
    }

    template<typename T>
    void LookupValueWithDefault(const libconfig::Setting & sSetting, const char* name, T& value, T defaultValue) {
        if(!sSetting.exists(name)) {
            _ssLog<< "[" << GetPluginName() << "]setting="<<name
                    <<"|defalue="<<defaultValue <<std::endl;
            value = defaultValue;
            return;
        }
        sSetting.lookupValue(name, value);
        _ssLog<< "[" << GetPluginName() << "]setting="<<name
            <<"|value="<< value <<std::endl;
        return;
    } 
};

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_CONFPLUGIN_H_