#ifndef _TITANS_SPP_PLUGIN_HIPPO_CONSUMER_PLUGIN_HIPPO_CONSUMER_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_HIPPO_CONSUMER_PLUGIN_HIPPO_CONSUMER_PLUGIN_H_

#include <Thread_hippo.h>
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

    ERspHandleStatus HandleHippo(const string& topic, const string& input, string& output, CServerBase* base);

protected:
    spp::ThreadPool m_thread_pool;
    HippoReader m_hippo_reader;
    std::string strHippoTopicMgrPath;
    std::map<std::string, BaseServiceRpc*> m_service;
    std::vector<stServiceConfig_t> m_serviceConfigure;
};

typedef Singleton<HippoConsumerPlugin> sHippoConsumerPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_HIPPO_CONSUMER_PLUGIN_HIPPO_CONSUMER_PLUGIN_H_