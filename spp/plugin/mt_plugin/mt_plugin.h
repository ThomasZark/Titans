#ifndef _TITANS_SPP_PLUGIN_MT_PLUGIN_MT_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_MT_PLUGIN_MT_PLUGIN_H_

#include <core/common/Singleton.h>
#include <spp/plugin/TmplPlugin.h>

namespace TITANS {

namespace PLUGIN {

class MtPlugin: public TmplPlugin {

public:
    MtPlugin();

    virtual ~MtPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    virtual void Finalize(void* arg1, void* arg2);

    virtual bool LoadParamConfig(const libconfig::Setting & sSetting);

protected:
    int iMtThreadNum;
};

typedef Singleton<MtPlugin> sMtPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_MT_PLUGIN_MT_PLUGIN_H_