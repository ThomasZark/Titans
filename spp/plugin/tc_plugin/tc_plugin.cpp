#include "tc_api.h"
#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <spp/plugin/tc_plugin/tc_plugin.h>

using namespace TITANS::PLUGIN;

TCPlugin::TCPlugin() {
    m_init = false;
}

TCPlugin::~TCPlugin() {

}

int TCPlugin::Initialize(void* arg1, void* arg2) {

    const char* etc = static_cast<char*>(arg1);
    CServerBase* base = static_cast<CServerBase*>(arg2);
    
    if(base->servertype() == SERVER_TYPE_WORKER) {
        if(!LoadConfig(etc)) {
            return -10002;
        }
        int ret = tc_api_init(strTDBankConfigPath.c_str());                                                                                                                                                
        if (0 != ret) {
            _ssLog << "init tc_api failed|" << ret << std::endl;
            return 10006; 
        } 
        m_init = true;
    }
    return 0;
}

void TCPlugin::Finalize(void* arg1, void* arg2) {

    CServerBase* base = static_cast<CServerBase*>(arg2);
    if (base->servertype() == SERVER_TYPE_WORKER) {
        tc_api_close(5000);
    }
}

int TCPlugin::ReportTDBank(const std::string& str) {
    
    if(!m_init) {
        return -1;  
    }
    if(!iTdbankTest) {
       return tc_api_send(strTDBankBid.c_str(), strTDBankTid.c_str(), str.c_str(), str.size());
    }
    return 0;
}

bool TCPlugin::LoadParamConfig(const libconfig::Setting & sSetting) {

    LookupValueWithDefault(sSetting, "strTDBankConfigPath", strTDBankConfigPath, std::string("../conf/config.json"));
    LookupValueWithDefault(sSetting, "strTDBankBid", strTDBankBid, std::string("b_sng_im_personal_live"));
    LookupValueWithDefault(sSetting, "strTDBankTid", strTDBankTid, std::string("personal_live_base"));
    LookupValueWithDefault(sSetting, "iTdbankTest", iTdbankTest, 1);
    return true;
}

REGIST_PLUGIN(TCPlugin)