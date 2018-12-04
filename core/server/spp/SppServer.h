#ifndef _TITANS_CORE_SERVER_SPP_SPPSERVER_H_
#define _TITANS_CORE_SERVER_SPP_SPPSERVER_H_

#include <core/common/Singleton.h>
#include <core/server/BaseServer.h>

namespace TITANS {

namespace SERVER {

struct stSppSvrContext: public stBaseSvrContext {
    void* arg1;
    void* arg2;
};

class SppServer: public BaseSever {

public:
    SppServer();

    virtual ~SppServer();

    //@desc - 服务初始化
    //@param
    //  pContext    - 初始化参数
    //@return
    //  int     - 0 成功
    virtual int Initialize(struct stBaseSvrContext* pContext);

    //@desc - 服务结束
    //@param
    //  pContext    - 初始化参数
    //@return
    //  int     - 0 成功
    virtual void Finalize(struct stBaseSvrContext* pContext);

    //@desc - 开始服务
    //@param
    //  pContext    - 初始化参数
    //@return
    //  int     - 0 成功
    virtual int Serve(struct stBaseSvrContext* pContext);

    //@desc - spp报文合法性检查回调接口
    //@param
    //@return
    //  int   ==0  数据包还未完整接收,继续等待
    //        > 0  数据包已经接收完整, 返回包长度
    //        < 0  数据包非法, 连接异常, 将断开TCP连接
    virtual int HandleSppInitCB(void* arg1, void* arg2);

    //@desc - spp初设化回调接口
    //@param
    //@return
    //  int     - 0 成功
    virtual int HandleSppInputCB(unsigned flow, void* arg1, void* arg2);

    //@desc - spp分发接口回调接口
    //@param
    //@return
    //  int     - 处理该报文的worker组id
    virtual int HandleSppRouteCB(unsigned flow, void* arg1, void* arg2);

    //@desc - spp业务处理回调接口
    //@param
    //@return
    //  int     - 0 成功
    virtual int HandleSppProcessCB(unsigned flow, void* arg1, void* arg2);

    //@desc - spp服务终止回调接口
    //@param
    //@return
    virtual void HandleSppFiniCB(void* arg1, void* arg2);

    //@desc - 流水日志时间格式
    //@param
    //@return
    //  int     - 0 成功
    char* format_time( time_t tm);

};

} // namespace SERVER

}//namespace TITANS

#endif //_TITANS_CORE_SERVER_SPP_SPPSERVER_H_