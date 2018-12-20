#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <spp/plugin/hippo_consumer_plugin/hippo_consumer_plugin.h>

using namespace TITANS::PLUGIN;

HippoConsumerPlugin::HippoConsumerPlugin() {

}

HippoConsumerPlugin::~HippoConsumerPlugin() {

}

int HippoConsumerPlugin::Initialize(void* arg1, void* arg2) {

    const char* etc = static_cast<char*>(arg1);
    CServerBase* base = static_cast<CServerBase*>(arg2);
    
    if(base->servertype() == SERVER_TYPE_PROXY) {
        if(!LoadConfig(etc)) {
            return -10002;
        }
        const int iRet = m_hippo_reader.Init(strHippoTopicMgrPath.c_str(), m_hippo_action, base);
        if(iRet < 0) {
            _ssLog << "hippo reader init failed|ret=" << iRet 
                    << "|err=" << m_hippo_reader.GetErrMsg() 
                    << std::endl;
            return iRet;
        }
        _ssLog << "hippo reader pool start" << std::endl; 
    }
    return 0;
}

bool HippoConsumerPlugin::LoadParamConfig(const libconfig::Setting & sSetting) {

    LookupValueWithDefault(sSetting, "strHippoTopicMgrPath", strHippoTopicMgrPath, std::string("./HippoMgr.conf"));
    return true;
}

int HippoConsumerPlugin::SetHippoAction(DoHippoAction action) {

    m_hippo_action = action;
    return 0;
}

REGIST_PLUGIN(HippoConsumerPlugin)