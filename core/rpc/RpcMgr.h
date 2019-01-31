#ifndef _TITANS_CORE_RPC_RPCMGR_H_
#define _TITANS_CORE_RPC_RPCMGR_H_

#include <map>
#include <vector>
#include <algorithm>
#include <libconfig.h++>
#include <sstream>

#include <core/common/Singleton.h>
#include <core/Rpc/BaseRpc.h>

namespace TITANS {

namespace RPC {

class RpcConf {

public:
    std::string name;
    uint32_t priority;
    
    friend bool operator < (const RpcConf& left, const RpcConf& right){
        return left.priority < right.priority;
    }
};

class RpcMgr {

public:
    RpcMgr() {}
    virtual ~RpcMgr() {}

    //@desc - rpc管理器初始化
    //@param
    //  arg1    - 参数1
    //  arg2    - 参数2
    //@return
    //  int     - 0 成功
    virtual int Initialize(void* arg1, void* arg2) {

        const char * etc  = const_cast<const char*>(static_cast<char*>(arg1));
        if(!LoadConfig(etc)) {
            _ssLog<<"RpcMgr conf init failed"<<std::endl;
            return -1;
        }
        std::sort(_rpcConf.begin(), _rpcConf.end());
        for(auto iter = _rpcConf.begin(); iter != _rpcConf.end(); iter ++) {
            auto rpcImpl = _rpcMap.find(iter->name);
            if(rpcImpl == _rpcMap.end()) {
                _ssLog<<"Rpc "<<iter->name<<" not found"<<std::endl;
                return -2;
            }
            rpcImpl->second->SetRpcName(iter->name);
            int ret = rpcImpl->second->Initialize(arg1, arg2);
            if(ret != 0) {
                _ssLog<<"Rpc "<<iter->name
                        <<" init failed, ret="
                        <<ret
                        <<", log="
                        <<std::endl
                        <<rpcImpl->second->GetLogInfo();
                return ret;
            }
            _ssLog<<"Rpc "<<iter->name
                    <<" init succ!"
                    <<", log="
                    <<std::endl
                    <<rpcImpl->second->GetLogInfo();
        }
        return 0;
    } 

    //@desc - rpc管理器退出
    //@param
    //  arg1    - 参数1
    //  arg2    - 参数2
    //@return
    virtual void Finalize(void* arg1, void* arg2) { 
        
        for(auto iter = _rpcMap.begin(); iter != _rpcMap.end(); iter ++) {
            iter->second->Finalize(arg1, arg2);
        }
        return; 
    }

    //@desc - rpc注册
    //@param
    //  pName     - rpc名称
    //  pRpc   - rpc实例指针
    //@return
    //  int     - 0 成功
    virtual int Regist(const char* pName, BaseRpc* pBaseRpc) {
        
        std::string name = pName;
        _rpcMap[name] = pBaseRpc;
        return 0;
    }

    //@desc - 获取日志信息
    //@param
    //@return
    //  std::string     - 0 日志
    virtual std::string GetLogInfo() {

        return _ssLog.str();
    }

protected:
    std::map<std::string, BaseRpc*> _rpcMap;
    std::vector<RpcConf> _rpcConf;
    std::stringstream _ssLog;

    bool LoadConfig(const char* etc) {
        try {
            libconfig::Config m_oCfg;
            m_oCfg.readFile(etc);
            const libconfig::Setting &Rpc_array = m_oCfg.lookup("rpc_mgr");
            for(int index = 0; index < Rpc_array.getLength(); index ++) {
                RpcConf conf;
                if(!Rpc_array[index].lookupValue("name", conf.name)
                    ||!Rpc_array[index].lookupValue("priority", conf.priority)) {
                    _ssLog<<"look Rpc conf falied, Rpc_length="<<Rpc_array.getLength()
                            <<", index="<<index
                            <<std::endl;
                    return false;
                }
                _ssLog<<"look Rpc conf, index="<<index
                        <<", priority="<<conf.priority
                        <<", Rpc="<<conf.name
                        <<std::endl;;
                _rpcConf.push_back(conf);
            }
            
        } catch (const libconfig::ParseException &e) {
            _ssLog<< "parse failed, err=" <<e.what() <<std::endl;
            return false;
        } catch (const libconfig::SettingException &e) {
            _ssLog<< "setting error=" << e.what() <<std::endl;
            return false;
        } catch (const std::exception &e){
            _ssLog<< "error=" << e.what() <<std::endl;
            return false;
        }
        return true;
    }
};

typedef Singleton<RpcMgr> sRpcMgr;

#define RPC_CAT(file, line) file##line
#define REGIST_RPC(Rpc)    \
static int RPC_CAT(R, __LINE__) = TITANS::RPC::sRpcMgr::Instance()->Regist(#Rpc, RPC_CAT(s, Rpc)::Instance());


} //namespace RPC

} //namespace TITANS

#endif //_TITANS_CORE_RPC_RPCMGR_H_