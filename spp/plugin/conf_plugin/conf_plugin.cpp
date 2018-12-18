#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <spp/plugin/conf_plugin/conf_plugin.h>

using namespace TITANS::PLUGIN;

ConfPlugin::ConfPlugin() {

}

ConfPlugin::~ConfPlugin() {

}

int ConfPlugin::Initialize(void* arg1, void* arg2) {

    const char* etc = static_cast<char*>(arg1);
    if(!LoadConfig(etc)) {
        return -10002;
    }

    if(!LoadConf()) {
        _ssLog<< "init conf plugin, LoadConf error"<< std::endl;
        return -10003;
    }
    return 0;
}

bool ConfPlugin::LoadParamConfig(const libconfig::Setting & sSetting) {

    LookupValueWithDefault(sSetting, "strConfPath", strConfPath, std::string("../conf/svr.conf"));
    return true;
}

bool ConfPlugin::LoadConf() {
    
    return true;
}

REGIST_PLUGIN(ConfPlugin)