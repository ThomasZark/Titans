#ifndef _TITANS_PLUGIN_MTPLUGIN_MT_PLUGIN_H_
#define _TITANS_PLUGIN_MTPLUGIN_MT_PLUGIN_H_

#include <core/common/Singlton.h>
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

#endif //_TITANS_PLUGIN_MTPLUGIN_MT_PLUGIN_H_