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
        if(!LoadConfig(etc)) {
            return -10002;
        }
        for(auto iter = m_serviceConfigure.begin(); iter != m_serviceConfigure.end(); iter ++) {
            auto plugin = m_service.find(iter->service);
            if(plugin == m_service.end()) {
                _ssLog<< "Service=" << iter->service << " not found" << std::endl;
                return -10005;
            }
            int ret = plugin->second->Init(m_Rpc, iter->service, *iter);
            if(ret != 0) {
                _ssLog<< "service=" << plugin->first << "init failed, ret=" << ret << std::endl;
                return ret;
            }
            _ssLog<< "service=" << plugin->first << "init succ" << std::endl;
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

bool RpcPlugin::LoadParamConfig(const libconfig::Setting & sSetting) {

    LookupValueWithDefault(sSetting, "strRpcMgrPath", m_strConfigPath, std::string("../conf/RpcMgr.conf"));
    return LoadServiceConfig(m_strConfigPath.c_str());
}

bool RpcPlugin::LoadServiceConfig(const char* etc) {
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
                ||!rpc_array[index].lookupValue("monitor", conf.monitor)
                ||!rpc_array[index].lookupValue("service", conf.service)) {
                _ssLog << "look rpc conf falied|" << rpc_array.getLength() << "|" << index << std::endl;
                return false;
            }
            _ssLog << "look rpc conf|" << index << "|" 
                    << conf.l5_mid << "|" 
                    << conf.l5_cid << "|" 
                    << conf.timeout << "|" 
                    << conf.net << "|" 
                    << conf.monitor << "|" 
                    << conf.service.c_str() << std::endl;
            m_serviceConfigure.push_back(conf);
        }
        
    } catch (const libconfig::ParseException &e) {
        _ssLog<< "[RPCConfig]parse failed, err=" <<e.what() <<std::endl;
        return false;
    } catch (const libconfig::SettingException &e) {
        _ssLog<< "[RPCConfig]setting error=" << e.what() <<std::endl;
        return false;
    } catch (const std::exception &e){
        _ssLog<< "[RPCConfig]error=" << e.what() <<std::endl;
        return false;
    }
    return true;
}

TASK::CRpc& RpcPlugin::Rpc() {

    return m_Rpc;
}

REGIST_PLUGIN(RpcPlugin)