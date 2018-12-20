#ifndef _TITANS_SPP_PLUGIN_HIPPO_CONSUMER_PLUGIN_HIPPO_CONSUMER_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_HIPPO_CONSUMER_PLUGIN_HIPPO_CONSUMER_PLUGIN_H_

#include <hippo_helper.h>
#include <core/common/Singleton.h>
#include <spp/plugin/TmplPlugin.h>

namespace TITANS {

namespace PLUGIN {

class HippoConsumerPlugin:public TmplPlugin {

public:
    HippoConsumerPlugin();

    virtual ~HippoConsumerPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    virtual bool LoadParamConfig(const libconfig::Setting & sSetting);

    int SetHippoAction(DoHippoAction action);

protected:
    spp::ThreadPool m_thread_pool;
    SppCommonHippoHelper m_hippo_reader;
    std::string strHippoTopicMgrPath;
    DoHippoAction m_hippo_action;
};

typedef Singleton<HippoConsumerPlugin> sHippoConsumerPlugin;

#define HIPPO_CAT(file, line) file##line
#define REGIST_HIPPO(action) \
static int HIPPO_CAT(R, __LINE__) = TITANS::PLUGIN::sHippoConsumerPlugin::Instance()->SetHippoAction(action);

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_HIPPO_CONSUMER_PLUGIN_HIPPO_CONSUMER_PLUGIN_H_