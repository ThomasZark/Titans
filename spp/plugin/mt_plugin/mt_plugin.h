#ifndef _TITANS_SPP_PLUGIN_MT_PLUGIN_MT_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_MT_PLUGIN_MT_PLUGIN_H_

#include <core/common/Singleton.h>
#include <core/plugin/Plugin.h>

namespace TITANS {

namespace PLUGIN {

class MtPlugin: public Plugin {

public:
    MtPlugin();

    virtual ~MtPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    virtual void Finalize(void* arg1, void* arg2);

};

typedef Singleton<MtPlugin> sMtPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_MT_PLUGIN_MT_PLUGIN_H_