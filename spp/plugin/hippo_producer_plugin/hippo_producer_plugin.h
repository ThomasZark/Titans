#ifndef _TITANS_SPP_PLUGIN_HIPPO_PRODUCER_PLUGIN_HIPPO_PRODUCER_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_HIPPO_PRODUCER_PLUGIN_HIPPO_PRODUCER_PLUGIN_H_

#include <Thread_hippo.h>
#include <core/common/Singleton.h>
#include <spp/plugin/TmplPlugin.h>

namespace TITANS {

namespace PLUGIN {

class HippoProducerPlugin :public TmplPlugin{

public:
    HippoProducerPlugin();

    virtual ~HippoProducerPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    virtual bool LoadParamConfig(const libconfig::Setting & sSetting);

    void SendHippoMsg(const std::string& hippo_data);
    
    void Callback(const ProducerResult &sendResult);

    void Monitor(uint32_t attr, uint32_t time);

protected:
    HippoSender m_hippo_sender;
    std::string strSenderGroupName;
    std::string strSenderTopic;
    std::string strHippoConf;
    int iTest;
    int iMonitor;

    enum Attr_Type {
        MONITOR_CALLBACK_SET_HIPPO_MSG_ASYNC_SUCC = 0,	            // hippo生产消息成功
        MONITOR_CALLBACK_RETRY_SET_HIPPO_MSG_ASYNC_FAIL = 1,	    // hippo回调函数中生产消息失败
        MONITOR_CALLBACK_SET_HIPPO_MSG_ASYNC_RESULT_FAIL = 2,	    // hippo生产消息失败
        MONITOR_CALLBACK_RETRY_SET_HIPPO_MSG_ASYNC_SUCC = 3,	    // hippo回调函数中生产消息成功
        MONITOR_RETRY_SET_HIPPO_MSG_ASYNC_FAIL = 4,                 // 生产消息失败
        MONITOR_SET_HIPPO_MSG_ASYNC_SUCC = 5,	                    // hippo异步发送消息成功
        MONITOR_HIPPO_MSG_ASYNC_SEND = 6,	                        // hippo生产消息
        MONITOR_RETRY_SET_HIPPO_MSG_ASYNC_SUCC = 7,	                // hippo发送消息重试        
    };
};
  
typedef Singleton<HippoProducerPlugin> sHippoProducerPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_HIPPO_PRODUCER_PLUGIN_HIPPO_PRODUCER_PLUGIN_H_