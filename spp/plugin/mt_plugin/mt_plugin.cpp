#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <spp/plugin/mt_plugin/mt_plugin.h>

using namespace TITANS::PLUGIN;

MtPlugin::MtPlugin() {

}

MtPlugin::~MtPlugin() {

}

int MtPlugin::Initialize(void* arg1, void* arg2) {

    const char* etc = static_cast<char*>(arg1);
    CServerBase* base = static_cast<CServerBase*>(arg2);

    if(base->servertype() == SERVER_TYPE_WORKER) {
        if(!LoadConfig(etc)) {
            return -10002;
        }
        int ret = CSyncFrame::Instance()->InitFrame(base, iMtThreadNum);
        if (ret < 0) {
            _ssLog<< "Sync framework init failed, ret=" << ret <<std::endl;
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

bool MtPlugin::LoadParamConfig(const libconfig::Setting & sSetting) {

    LookupValueWithDefault(sSetting, "iMtThreadNum", iMtThreadNum, 100000);
    return true;
}

REGIST_PLUGIN(MtPlugin)