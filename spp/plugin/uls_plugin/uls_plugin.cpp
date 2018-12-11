#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <spp/plugin/uls_plugin/uls_plugin.h>

using namespace TITANS::PLUGIN;

ULSPlugin::ULSPlugin() {

}

ULSPlugin::~ULSPlugin() {

}

int ULSPlugin::Initialize(void* arg1, void* arg2) {

    CServerBase* base = static_cast<CServerBase*>(arg2);

    if(base->servertype() == SERVER_TYPE_WORKER) {
        int uls_ret = uls::UlsLogger::init_uls("../log/", 0x940003, 107, 2, 1, 1, false);
        if(0 != uls_ret){
            base->log_.LOG_P_PID(LOG_ERROR, "init uls log error|ret=%d|\n", uls_ret);
            return uls_ret;
        }
    }
    return 0;
}

REGIST_PLUGIN(UlsPlugin)