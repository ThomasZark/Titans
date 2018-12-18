#ifndef _TITANS_SPP_PLUGIN_TC_PLUGIN_TC_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_TC_PLUGIN_TC_PLUGIN_H_

#include <core/common/Singleton.h>
#include <spp/plugin/TmplPlugin.h>

namespace TITANS {

namespace PLUGIN {

class TCPlugin:public TmplPlugin {

public:
    TCPlugin();

    virtual ~TCPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    virtual void Finalize(void* arg1, void* arg2);

    virtual bool LoadParamConfig(const libconfig::Setting& sSetting);

    int ReportTDBank(const std::string& str);

protected:
    bool m_init;
    std::string strTDBankConfigPath;
    std::string strTDBankBid;
    std::string strTDBankTid;
    int iTdbankTest;                    // 1上报屏蔽; 0 上报开启    
};
  
typedef Singleton<TCPlugin> sTCPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_TC_PLUGIN_TC_PLUGIN_H_