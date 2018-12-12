#ifndef _TITANS_CORE_PLUGIN_PLUGIN_H_
#define _TITANS_CORE_PLUGIN_PLUGIN_H_

namespace TITANS {

namespace PLUGIN {

class Plugin {

public:
    Plugin() {}

    virtual ~Plugin() {}

    //@desc - 插件初始化
    //@param
    //  arg1    - 参数1
    //  arg2    - 参数2
    //@return
    //  int     - 0 成功
    virtual int Initialize(void* arg1, void* arg2) { return 0; }

    //@desc - 插件退出
    //@param
    //  arg1    - 参数1
    //  arg2    - 参数2
    //@return
    virtual void Finalize(void* arg1, void* arg2) { return; }

    //@desc - 设置插件名称
    //@param
    //  strName - 插件名称
    //@return
    virtual void SetPluginName(std::string& strName) {
        _pluginName = strName;
    }

    //@desc - 获取插件名称
    //@param
    //@return
    virtual const std::string& GetPluginName() {
        return _pluginName;
    }

    //@desc - 获取日志信息
    //@param
    //@return
    //  std::string     - 0 日志
    virtual std::string GetLogInfo() {
        return _ssLog.str();
    }

protected:
    std::stringstream _ssLog;
    std::string _pluginName;
};

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_CORE_PLUGIN_PLUGIN_H_