#ifndef _TITANS_SPP_PLUGIN_FLOW_PLUGIN_FLOW_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_FLOW_PLUGIN_FLOW_PLUGIN_H_

#include <core/common/Singleton.h>
#include <spp/plugin/TmplPlugin.h>
#include <syncincl.h>

namespace TITANS {

namespace PLUGIN {

class FlowPlugin:public TmplPlugin {

public:
    FlowPlugin();

    virtual ~FlowPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    virtual bool LoadParamConfig(const libconfig::Setting& sSetting);

    CTLog flow_log;

    virtual bool IsInit();

protected:
    bool m_init;
    std::string strFlowLogPath;     //流水日志路径
    std::string strFlowLogPrefix;   //流水日志文件前缀
    int iFlowLogLevel;              //流水日志等级
    int iMaxFilesSize;              //流水日志文件大小
    int iMaxFilesNum;               //流水日志文件数量
    int iLogType;                   //流水日志类型
};

#define FLOW_LOG(lvl, fmt, args...)                                                                             \
do {                                                                                                            \
    if(sFlowPlugin::Instance()->IsInit() && lvl >= sFlowPlugin::Instance()->flow_log.log_level(-1)) {             \
        sFlowPlugin::Instance()->flow_log.LOG_P_ALL(lvl, fmt"\n", ##args);                                       \
    }                                                                                                           \
} while (0)

typedef Singleton<FlowPlugin> sFlowPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_FLOW_PLUGIN_FLOW_PLUGIN_H_