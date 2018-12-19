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

    void Monitor(uint32_t attr, uint32_t time);

protected:
    bool m_init;
    int iMonitor;
    std::string strTDBankConfigPath;
    std::string strTDBankBid;
    std::string strTDBankTid;
    int iTdbankTest;                    // 1上报屏蔽; 0 上报开启    

    enum Attr_Type {
        MONITOR_REPORT = 0,             //tdbank上报
        MONITOR_REPORT_FAILED = 1,      //tdbank上报失败
    };
};
  
typedef Singleton<TCPlugin> sTCPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_TC_PLUGIN_TC_PLUGIN_H_