#include <syncincl.h>

#include <core/plugin/PluginMgr.h>
#include <spp/plugin/rpc_plugin/rpc_plugin.h>

using namespace TITANS::PLUGIN;

RpcPlugin::RpcPlugin() {

}

RpcPlugin::~RpcPlugin() {

}

int RpcPlugin::Initialize(void* arg1, void* arg2) {

    char* etc = static_cast<char*>(arg1);
    CServerBase* base = static_cast<CServerBase*>(arg2);

    if (base->servertype() == SERVER_TYPE_WORKER) {
        
        if(!LoadConfig(etc, base)) {
            base->log_.LOG_P_PID(LOG_FATAL, "RpcPlugin conf init failed\n");
            return -1;
        }
        for(auto iter = m_serviceConfigure.begin(); iter != m_serviceConfigure.end(); iter ++) {
            auto plugin = m_service.find(iter->service);
            if(plugin == m_service.end()) {
                base->log_.LOG_P_PID(LOG_FATAL, "Service %s not found\n", iter->service.c_str());
                return -2;
            }
            int ret = plugin->second->Init(m_Rpc, iter->service, *iter);
            if(ret != 0) {
                base->log_.LOG_P_PID(LOG_FATAL, "service %s init failed, ret=%d\n", plugin->first.c_str(), ret);
                return ret;
            }
            base->log_.LOG_P_PID(LOG_NORMAL, "service %s init succ\n", plugin->first.c_str());
        }
        return 0;
    }
    return 0;
}

int RpcPlugin::Regist(const char* pName, RPC::BaseServiceRpc* pService) {
    
    std::string name = pName;
    m_service[name] = pService;
    SF_LOG(LOG_NORMAL, "Regist Service|%s|\n", pName);
    return 0;
}

bool RpcPlugin::LoadConfig(const char* etc, CServerBase* base) {
    try {
        libconfig::Config m_oCfg;
        m_oCfg.readFile(etc);
        const libconfig::Setting &rpc_array = m_oCfg.lookup("service_config");
        for(int index = 0; index < rpc_array.getLength(); index ++) {
            RPC::stServiceConfig_t conf;
            if(!rpc_array[index].lookupValue("l5_mid", conf.l5_mid)
                ||!rpc_array[index].lookupValue("l5_cid", conf.l5_cid)
                ||!rpc_array[index].lookupValue("timeout", conf.timeout)
                ||!rpc_array[index].lookupValue("net", conf.net)
                ||!rpc_array[index].lookupValue("service", conf.service)) {
                base->log_.LOG_P_PID(LOG_ERROR, "look rpc conf falied|%d|%d|", rpc_array.getLength(), index);
                return false;
            }
            base->log_.LOG_P_PID(LOG_DEBUG, "look rpc conf|%d|%d|%d|%d|%d|%s\n", index, conf.l5_mid, conf.l5_cid, conf.timeout, conf.net, conf.service.c_str());
            m_serviceConfigure.push_back(conf);
        }
        
    } catch (const libconfig::ParseException &e) {
        base->log_.LOG_P_PID(LOG_ERROR, "parse failed|err=%s|", e.what());
        return false;
    } catch (const libconfig::SettingException &e) {
        base->log_.LOG_P_PID(LOG_ERROR, "setting|error=%s|", e.what());
        return false;
    } catch (const std::exception &e){
        base->log_.LOG_P_PID(LOG_ERROR, "error=%s", e.what());
        return false;
    }
    return true;
}

TASK::CRpc& RpcPlugin::Rpc() {

    return m_Rpc;
}

REGIST_PLUGIN(RpcPlugin)