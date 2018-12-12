#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <spp/plugin/uls_plugin/uls_plugin.h>

using namespace TITANS::PLUGIN;

ULSPlugin::ULSPlugin() {

}

ULSPlugin::~ULSPlugin() {

}

int ULSPlugin::Initialize(void* arg1, void* arg2) {

    const char* etc = static_cast<char*>(arg1);
    CServerBase* base = static_cast<CServerBase*>(arg2);
    if(!LoadConfig(etc)) {
        return -10002;
    }
    if(base->servertype() == SERVER_TYPE_WORKER) {
        int uls_ret = uls::UlsLogger::init_uls(strUlsPath, iUlsId, iWaterLogId, iColorLogId, iWaterLogFlag, iColorLogFlag, bOpenLocalLog);
        if(0 != uls_ret){
            _ssLog<< "init uls log error|ret=" << uls_ret << std::endl;
            return uls_ret;
        }
    }
    return 0;
}

bool ULSPlugin::LoadParamConfig(const libconfig::Setting & sSetting) {

    LookupValueWithDefault(sSetting, "strUlsPath", strUlsPath, std::string("../log/"));
    LookupValueWithDefault(sSetting, "iUlsId", iUlsId, 0x940003);
    LookupValueWithDefault(sSetting, "iWaterLogId", iWaterLogId, 107);
    LookupValueWithDefault(sSetting, "iColorLogId", iColorLogId, 2);
    LookupValueWithDefault(sSetting, "iWaterLogFlag", iWaterLogFlag, 1);
    LookupValueWithDefault(sSetting, "iColorLogFlag", iColorLogFlag, 1);
    LookupValueWithDefault(sSetting, "bOpenLocalLog", bOpenLocalLog, false);
    return true;
}

REGIST_PLUGIN(UlsPlugin)