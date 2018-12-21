#include <spp/plugin/hippo_consumer_plugin/hippo_consumer_plugin.h>

using namespace TITANS::PLUGIN;

ERspHandleStatus HandleHippo(const string& topic, const string& input, string& output, CServerBase* base) {

    return NOT_NEED_RSP;
}

REGIST_HIPPO(HandleHippo)