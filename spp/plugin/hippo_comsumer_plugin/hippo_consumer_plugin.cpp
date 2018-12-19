#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <spp/plugin/hippo_comsumer_plugin.h>

using namespace TITANS::PLUGIN;

HippoConsumerPlugin::HippoConsumerPlugin() {

}

HippoConsumerPlugin::~HippoConsumerPlugin() {

}

int HippoConsumerPlugin::Initialize(void* arg1, void* arg2) {

    const char* etc = static_cast<char*>(arg1);
    CServerBase* base = static_cast<CServerBase*>(arg2);
    
    if(base->servertype() == SERVER_TYPE_PROXY) {
        if(!LoadConfig(etc)) {
            return -10002;
        }
        map<string, string> topic_lists;
        topic_lists[sSetting::Instance().strTopic] = sSetting::Instance().strGroupName;
        m_hippo_reader.AddTopic(topic_lists);

        auto functional = std::bind(&HippoConsumerPlugin::HandleHippo, 
                                    &sHippoConsumerPlugin::Instance(), 
                                    std::placeholders::_1, 
                                    std::placeholders::_2, 
                                    std::placeholders::_3,
                                    std::placeholders::_4);
        const int iRet = m_hippo_reader.realinit(functional, 
                                                base,
                                                sSetting::Instance().strHippoConf, 
                                                sSetting::Instance().uiSelfPort, 
                                                sSetting::Instance().uiTest, 
                                                sSetting::Instance().iConsumeWhere);
        if(iRet < 0) {
            base->log_.LOG_P_PID(LOG_FATAL, "hippo reader init failed|ret=%d|\n", iRet);
            return iRet;
        }

        m_thread_pool.start(&m_hippo_reader);
        base->log_.LOG_P_PID(LOG_ERROR, "hippo reader pool start\n"); 
    }
    return 0;
}

bool HippoConsumerPlugin::LoadParamConfig(const libconfig::Setting & sSetting) {

    LookupValueWithDefault(sSetting, "strHippoTopicMgrPath", strHippoTopicMgrPath, std::string("./HippoMgr.conf"));
    return LoadHippoTopicConfig(m_strConfigPath.c_str());
}

bool HippoConsumerPlugin::LoadHippoTopicConfig(const char* etc) {
    try {
        libconfig::Config m_oCfg;
        m_oCfg.readFile(etc);
        const libconfig::Setting &topic_array = m_oCfg.lookup("topic_config");
        for(int index = 0; index < topic_array.getLength(); index ++) {
            stTopicConfig_t conf;
            if(!topic_array[index].lookupValue("strTopic", conf.l5_mid)
                ||!topic_array[index].lookupValue("strGroup", conf.l5_cid)
                ||!topic_array[index].lookupValue("strHippoConf", conf.timeout)
                ||!topic_array[index].lookupValue("uiSelfPort", conf.net)
                ||!topic_array[index].lookupValue("uiTest", conf.service)
                ||!topic_array[index].lookupValue("iConsumeWhere", conf.service)) {
                _ssLog << "look rpc conf falied|" << topic_array.getLength() << "|" << index << std::endl;
                return false;
            }
            _ssLog << "look rpc conf|" << index << "|" 
                    << conf.strTopic << "|" 
                    << conf.strGroup << "|" 
                    << conf.strHippoConf << "|" 
                    << conf.uiSelfPort << "|" 
                    << conf.uiTest << "|" 
                    << conf.iConsumeWhere 
                    << std::endl;
            m_topicConfigure.push_back(conf);
        }
        
    } catch (const libconfig::ParseException &e) {
        _ssLog<< "[HippoTopicConfig]parse failed, err=" <<e.what() <<std::endl;
        return false;
    } catch (const libconfig::SettingException &e) {
        _ssLog<< "[HippoTopicConfig]setting error=" << e.what() <<std::endl;
        return false;
    } catch (const std::exception &e){
        _ssLog<< "[HippoTopicConfig]error=" << e.what() <<std::endl;
        return false;
    }
    return true;
}

ERspHandleStatus HippoConsumerPlugin::HandleHippo(const string& topic, const string& input, string& output, CServerBase* base)
{
    return NOT_HANDLE;
}

REGIST_PLUGIN(HippoConsumerPlugin)