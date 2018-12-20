#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <spp/plugin/hippo_producer_plugin/hippo_producer_plugin.h>

using namespace TITANS::PLUGIN;

HippoProducerPlugin::HippoProducerPlugin() {

}

HippoProducerPlugin::~HippoProducerPlugin() {

}

int HippoProducerPlugin::Initialize(void* arg1, void* arg2) {

	const char* etc = static_cast<char*>(arg1);
	CServerBase* base = static_cast<CServerBase*>(arg2);
    if (base->servertype() == SERVER_TYPE_WORKER) {
        if(!LoadConfig(etc)) {
        	return -10002;
    	}
        auto func_cb = std::bind(&HippoProducerPlugin::Callback, 
                                sHippoProducerPlugin::Instance(),
                                std::placeholders::_1);
        bool ret = m_hippo_sender.init(strSenderGroupName, 
                                        strSenderTopic, 
                                        strHippoConf, 
                                        (PProducerCallBack)func_cb, 
                                        iTest > 0);
        if(!ret) {
			_ssLog << "hippo_sender init failed|errmsg=" << m_hippo_sender.get_errmsg() << std::endl;
			return -10004;
		}
    }
	_ssLog << "hippo_sender init succ" << std::endl;
    return 0;  
}

void HippoProducerPlugin::Callback(const ProducerResult &sendResult) {
    
    if (!sendResult.isSuccess()) {
		SF_LOG(LOG_ERROR, "[Callback]SendMessage failed!|result=%d|errmsg=%s|", sendResult.GetErrCode(), sendResult.GetErrMessage().c_str());
		Monitor(MONITOR_CALLBACK_SET_HIPPO_MSG_ASYNC_RESULT_FAIL, 1);  //hippo生产消息失败

		string hippo_data;
		hippo_data.assign((const char*)sendResult.getUserMessage().getData(), sendResult.getUserMessage().getDataLength());

		int i;
		for (i = 0 ;i < iMaxHippoTryTimes; i++) {
			int hippoSucc = m_hippo_sender.set_HippoMsgAsync(hippo_data);
			if (hippoSucc) {
				Monitor(MONITOR_CALLBACK_RETRY_SET_HIPPO_MSG_ASYNC_SUCC, 1);  //hippo回调函数中生产消息成功
				SF_LOG(LOG_DEBUG, "[Callback]set_HippoMsg succ!");
				return;
			}
		}
		if (i >= iMaxHippoTryTimes) {
			Monitor(MONITOR_CALLBACK_RETRY_SET_HIPPO_MSG_ASYNC_FAIL, 1);  //hippo回调函数中生产消息失败
			SF_LOG(LOG_ERROR, "[Callback]set_HippoMsg failed!|errcode=%d|errmsg=%s|", m_hippo_sender.get_errcode(), m_hippo_sender.get_errmsg().c_str());
			return;
		}
		return;
	}
	SF_LOG(LOG_NORMAL, "[Callback]Send Hippo msg successfully!");
	Monitor(MONITOR_CALLBACK_SET_HIPPO_MSG_ASYNC_SUCC, 1);  //hippo生产消息成功
}

void HippoProducerPlugin::SendHippoMsg(const std::string& hippo_data) {

    Monitor(MONITOR_HIPPO_MSG_ASYNC_SEND, 1);  //hippo生产消息
	
    int i;
	for (i = 0; i < iMaxHippoTryTimes; i++) {
		bool hippoSucc = m_hippo_sender.set_HippoMsgAsync(hippo_data);
		if (hippoSucc) {
			if (0 == i){
				Monitor(MONITOR_SET_HIPPO_MSG_ASYNC_SUCC, 1); //hippo异步发送消息成功
			} else {
				Monitor(MONITOR_RETRY_SET_HIPPO_MSG_ASYNC_SUCC, 1); //hippo发送消息重试
			}
			return;
		}
	}
	if (i >= iMaxHippoTryTimes) {
		Monitor(MONITOR_RETRY_SET_HIPPO_MSG_ASYNC_FAIL, 1);  //生产消息失败
        SF_LOG(LOG_ERROR, "set_HippoMsg failed!|errcode=%d|errmsg=%s|", 
                m_hippo_sender.get_errcode(), m_hippo_sender.get_errmsg().c_str());
		return;
	}
	return;
}

void HippoProducerPlugin::Monitor(uint32_t attr, uint32_t time) {

	Attr_API(iMonitor + attr, time);
}

bool HippoProducerPlugin::LoadParamConfig(const libconfig::Setting & sSetting) {

    LookupValueWithDefault(sSetting, "strSenderGroupName", strSenderGroupName, std::string("b_sng_hippo_template_producer_group"));
	LookupValueWithDefault(sSetting, "strSenderTopic", strSenderTopic, std::string("b_sng_hippo_template_producer_topic"));
	LookupValueWithDefault(sSetting, "strHippoConf", strHippoConf, std::string("../conf/hippoclient.conf"));
	LookupValueWithDefault(sSetting, "iMaxHippoTryTimes", iMaxHippoTryTimes, 3);
	LookupValueWithDefault(sSetting, "iTest", iTest, 1);
	LookupValueWithDefault(sSetting, "iMonitor", iMonitor, 3472670);
    return true;
}

REGIST_PLUGIN(HippoProducerPlugin)