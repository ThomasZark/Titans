#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <plugin/mt_plugin/mt_plugin.h>

using namespace TITANS::PLUGIN;

MtPlugin::MtPlugin() {

}

MtPlugin::~MtPlugin() {

}

int MtPlugin::Initialize(void* arg1, void* arg2) {

    const char * etc  = const_cast<const char*>(static_cast<char*>(arg1));
    CServerBase* base = static_cast<CServerBase*>(arg2);

    if(base->servertype() == SERVER_TYPE_WORKER) {
        int ret = CSyncFrame::Instance()->InitFrame(base, 100000);
        if (ret < 0) {
            base->log_.LOG_P_PID(LOG_FATAL, "Sync framework init failed, ret:%d\n", ret);
            return -10001;
        }
    }
    return 0;
}

void MtPlugin::Finalize(void* arg1, void* arg2) {

    CServerBase* base = static_cast<CServerBase*>(arg2);
    if ( base->servertype() == SERVER_TYPE_WORKER) {
        CSyncFrame::Instance()->Destroy();
    }
}

REGIST_PLUGIN(sMtPlugin)