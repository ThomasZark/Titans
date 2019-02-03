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
            SSLOG()<<"pluginMgr conf init failed"<<std::endl;
            return -1;
        }
        std::sort(_pluginConf.begin(), _pluginConf.end());
        for(auto iter = _pluginConf.begin(); iter != _pluginConf.end(); iter ++) {
            auto plugin = _plugin.find(iter->name);
            if(plugin == _plugin.end()) {
                SSLOG()<<"plugin "<<iter->name<<" not found"<<std::endl;
                return -2;
            }
            plugin->second->SetPluginName(iter->name);
            int ret = plugin->second->Initialize(arg1, arg2);
            if(ret != 0) {
                SSLOG()<<"plugin "<<iter->name
                        <<" init failed, ret="
                        <<ret
                        <<", log="
                        <<std::endl
                        <<plugin->second->GetSSLOGInfo();
                return ret;
            }
            SSLOG()<<"plugin "<<iter->name
                    <<" init succ!"
                    <<", log="
                    <<std::endl
                    <<plugin->second->GetSSLOGInfo();
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
    virtual std::string GetSSLOGInfo() {

        return _ssLog.str();
    }

protected:
    //注册插件
    std::map<std::string, Plugin*> _plugin;
    //插件配置
    std::vector<PluginConf> _pluginConf;
    //日志
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
                    SSLOG()<<"look plugin conf falied, plugin_length="<<plugin_array.getLength()
                            <<", index="<<index
                            <<std::endl;
                    return false;
                }
                SSLOG()<<"look plugin conf, index="<<index
                        <<", priority="<<conf.priority
                        <<", plugin="<<conf.name
                        <<std::endl;;
                _pluginConf.push_back(conf);
            }
            
        } catch (const libconfig::ParseException &e) {
            SSLOG()<< "parse failed, err=" <<e.what() <<std::endl;
            return false;
        } catch (const libconfig::SettingException &e) {
            SSLOG()<< "setting error=" << e.what() <<std::endl;
            return false;
        } catch (const std::exception &e){
            SSLOG()<< "error=" << e.what() <<std::endl;
            return false;
        }
        return true;
    }
};

typedef Singleton<PluginMgr> sPluginMgr;

#define PLUGIN_CAT(file, line) file##line
#define REGIST_PLUGIN(Plugin)    \
static int PLUGIN_CAT(R, __LINE__) = TITANS::PLUGIN::sPluginMgr::Instance()->Regist(#Plugin, PLUGIN_CAT(s, Plugin)::Instance());


} //namespace PLUGIN

} //namespace TITANS

#endif //_TITANS_CORE_PLUGIN_PLUGINMGR_H_