#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <spp/plugin/flow_plugin/flow_plugin.h>

using namespace TITANS::PLUGIN;

FlowPlugin::FlowPlugin() {
    m_init = false;
}

FlowPlugin::~FlowPlugin() {

}

int FlowPlugin::Initialize(void* arg1, void* arg2) {

    const char* etc = static_cast<char*>(arg1);
    CServerBase* base = static_cast<CServerBase*>(arg2);
    if(!LoadConfig(etc)) {
        return -10002;
    }
    if(base->servertype() == SERVER_TYPE_WORKER) {
        flow_log.LOG_OPEN(iFlowLogLevel,
                        iLogType,
                        strFlowLogPath.c_str(),
                        strFlowLogPrefix.c_str(),
                        iMaxFilesSize,
                        iMaxFilesNum);
        m_init = true;
    }
    return 0;
}

bool FlowPlugin::LoadParamConfig(const libconfig::Setting & sSetting) {

    LookupValueWithDefault(sSetting, "strFlowLogPath", strFlowLogPath, std::string("../log"));
    LookupValueWithDefault(sSetting, "strFlowLogPrefix", strFlowLogPrefix, std::string("flow_log"));
    LookupValueWithDefault(sSetting, "iFlowLogLevel", iFlowLogLevel, 0);
    LookupValueWithDefault(sSetting, "iMaxFilesSize", iMaxFilesSize, 52428800);
    LookupValueWithDefault(sSetting, "iMaxFilesNum", iMaxFilesNum, 30);
    LookupValueWithDefault(sSetting, "iLogType", iLogType, 0);
    return true;
}

bool FlowPlugin::IsInit() {
    return m_init;
}

REGIST_PLUGIN(FlowPlugin)