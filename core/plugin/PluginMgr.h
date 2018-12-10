#ifndef _TITANS_CORE_PLUGIN_PLUGINMGR_H_
#define _TITANS_CORE_PLUGIN_PLUGINMGR_H_

#include <map>
#include <vector>
#include <algorithm>
#include <libconfig.h++>
#include <sstream>

#include <core/common/Singleton.h>
#include <core/plugin/Plugin.h>

namespace TITANS {

namespace PLUGIN {

class PluginConf {

public:
    std::string name;
    uint32_t priority;
    
    friend bool operator < (const PluginConf& left, const PluginConf& right){
        return left.priority < right.priority;
    }
};

class PluginMgr {

public:
    PluginMgr() {}
    virtual ~PluginMgr() {}

    //@desc - 插件管理器初始化
    //@param
    //  arg1    - 参数1
    //  arg2    - 参数2
    //@return
    //  int     - 0 成功
    virtual int Initialize(void* arg1, void* arg2) {

        const char * etc  = const_cast<const char*>(static_cast<char*>(arg1));
        if(!LoadConfig(etc)) {
            _ssLog<<"pluginMgr conf init failed"<<std::endl;
            return -1;
        }
        std::sort(_pluginConf.begin(), _pluginConf.end());
        for(auto iter = _pluginConf.begin(); iter != _pluginConf.end(); iter ++) {
            auto plugin = _plugin.find(iter->name);
            if(plugin == _plugin.end()) {
                _ssLog<<"plugin "<<iter->name<<" not found"<<std::endl;
                return -2;
            }
            int ret = plugin->second->Initialize(arg1, arg2);
            if(ret != 0) {
                _ssLog<<"plugin "<<iter->name<<" init failed, ret="<<ret<<std::endl;
                return ret;
            }
            _ssLog<<"plugin "<<iter->name<<" init succ!"<<std::endl;
        }
        return 0;
    } 

    //@desc - 插件管理器退出
    //@param
    //  arg1    - 参数1
    //  arg2    - 参数2
    //@return
    virtual void Finalize(void* arg1, void* arg2) { 
        
        for(auto iter = _plugin.begin(); iter != _plugin.end(); iter ++) {
            iter->second->Finalize(arg1, arg2);
        }
        return; 
    }

    //@desc - 插件注册
    //@param
    //  pName     - 插件名称
    //  pPlugin   - 插件实例指针
    //@return
    //  int     - 0 成功
    virtual int Regist(const char* pName, Plugin* pPlugin) {
        
        std::string name = pName;
        _plugin[name] = pPlugin;
        return 0;
    }

    //@desc - 获取日志信息
    //@param
    //@return
    //  std::string     - 0 日志
    virtual std::string GetLogInfo() {

        return _ssLog.str();
    }

protected:
    std::map<std::string, Plugin*> _plugin;
    std::vector<PluginConf> _pluginConf;
    std::stringstream _ssLog;
    
    bool LoadConfig(const char* etc) {
        try {
            libconfig::Config m_oCfg;
            m_oCfg.readFile(etc);
            const libconfig::Setting &plugin_array = m_oCfg.lookup("plugin_mgr");
            for(int index = 0; index < plugin_array.getLength(); index ++) {
                PluginConf conf;
                if(!plugin_array[index].lookupValue("name", conf.name)
                    ||!plugin_array[index].lookupValue("priority", conf.priority)) {
                    _ssLog<<"look plugin conf falied, plugin_length="<<plugin_array.getLength()
                            <<", index="<<index
                            <<std::endl;
                    return false;
                }
                _ssLog<<"look plugin conf, index="<<index
                        <<", priority="<<conf.priority
                        <<", plugin="<<conf.name
                        <<std::endl;;
                _pluginConf.push_back(conf);
            }
            
        } catch (const libconfig::ParseException &e) {
            _ssLog<< "parse failed, err=" <<e.what() <<std::endl;
            return false;
        } catch (const libconfig::SettingException &e) {
            _ssLog<< "setting error=" << e.what() <<std::endl;
            return false;
        } catch (const std::exception &e){
            _ssLog<< "error=" << e.what() <<std::endl;
            return false;
        }
        return true;
    }

};

typedef Singleton<PluginMgr> sPluginMgr;

#define CAT(file, line) file##line
#define REGIST_PLUGIN(Plugin)    \
static int CAT(R, __LINE__) = TITANS::PLUGIN::sPluginMgr::Instance()->Regist(#Plugin, CAT(s, Plugin)::Instance());


} //namespace PLUGIN

} //namespace TITANS

#endif //_TITANS_CORE_PLUGIN_PLUGINMGR_H_