#ifndef _TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVEMSGFATORY_H_
#define _TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVEMSGFATORY_H_

#include <functional>
#include <spp/handler/ctx/ilive_ctx/IliveMsg.h>

namespace TITANS {

namespace HANDLER {

typedef std::function<TITANS::HANDLER::IliveMsg* (void)> IliveMsgCreateFunc;

class IliveMsgFactory{

public:

    virtual ~IliveMsgFactory() {}

    static IliveMsgFactory* Instance() {
        static IliveMsgFactory obj;
        return &obj;
    }

    //@desc -初始化 
    //@param
    //  svr_cmd - BIG_CMD
    //  attr    - Monitor
    //@return
    //  int     - 0:OK
    int Init(uint32_t svr_cmd, uint32_t attr) {
        SVR_CMD = svr_cmd;
        ATTR = attr;
        for(auto iter = m_msg_func.begin(); iter != m_msg_func.end(); iter++) {
            INIT_STAT_ITEM(iter->first);
        }
        return 0;
    }

    //@desc - 根据子命令字创建消息类
    //@param
    //  data    - 请求包数据
    //  size    - 请求包长度
    //@return
    //  IliveMsg* - NULL 失败
    virtual IliveMsg* CreateMsg(const char* data, uint32_t size) {

        MONITOR_API(MONITOR_ALL); //总请求

        IliveCtxPtr pCtx = std::make_shared<IliveContext>();

        int ret = pCtx->ReqPkgDecode(data, size, NULL);
        if (0 != ret) {
            MONITOR_API(MONITOR_DECODE_REQ_FAILED);//解包失败
            MONITOR_API(MONITOR_FAILED);//总失败量
            SF_LOG(LOG_ERROR, "decode request failed|ret=%d|cmd=%d|sub_cmd=%d|uin=%zu|client_ip=%u", 
                ret, pCtx->HeadReq().cmd(), pCtx->HeadReq().subcmd(), pCtx->HeadReq().uid(), pCtx->HeadReq().client_ip());
            return NULL;
        }

        if (pCtx->HeadReq().cmd() != SVR_CMD) {
            MONITOR_API(MONITOR_WRONG_BIG_CMD);//错误的大命令字
            MONITOR_API(MONITOR_FAILED);//总失败量
            SF_LOG(LOG_ERROR, "decode request failed|cmd=%d|sub_cmd=%d|uin=%zu|client_ip=%u",
                pCtx->HeadReq().cmd(), pCtx->HeadReq().subcmd(), pCtx->HeadReq().uid(), pCtx->HeadReq().client_ip());
            return NULL;
        }

        SF_LOG(LOG_DEBUG, "subcmd=%d|uin=%zu|seq=%d|client_ip=%u", 
            pCtx->HeadReq().subcmd(), pCtx->HeadReq().uid(), pCtx->HeadReq().seq(), pCtx->HeadReq().client_ip());

        auto subcmd = pCtx->HeadReq().subcmd();
        auto iter = m_msg_func.find(subcmd);
        if(iter == m_msg_func.end()) { 
            //never got here
            MONITOR_API(MONITOR_WRONG_SUBCMD);//错误的子命令
            SF_LOG(LOG_ERROR, "wrong subcmd, subcmd=%d|uin=%zu|seq=%d|client_ip=%u", 
                pCtx->HeadReq().subcmd(), pCtx->HeadReq().uid(), pCtx->HeadReq().seq(), pCtx->HeadReq().client_ip());
            IliveMsg* sub_msg = m_msg_func[0]();
            int retcode = -1;
            sub_msg->Init(static_cast<void*>(&retcode));
            sub_msg->SetContext(pCtx);
            return sub_msg;
        }

        IliveMsg* sub_msg = iter->second();
        sub_msg->Init(static_cast<void*>(&ATTR));
        sub_msg->SetContext(pCtx);
        return sub_msg;
    }

    //@desc - 注册消息的生成函数
    //@param
    //  key     - 子命令字
    //  func    - 生成函数
    //@return
    //  int     - 0 成功
    virtual int Regist(int key, IliveMsgCreateFunc func) {
        m_msg_func[key] = func;
        return 0;
    }

private:

    uint32_t SVR_CMD;
    uint32_t ATTR;
    std::map<int, IliveMsgCreateFunc> m_msg_func;

    enum Attr_Type{
        MONITOR_ALL = 0,
        MONITOR_OK  = 1,
        MONITOR_FAILED = 2,
        MONITOR_WRONG_BIG_CMD = 3,
        MONITOR_WRONG_SUBCMD = 4,
        MONITOR_DECODE_REQ_FAILED = 5,
        MONITOR_ENCODE_RSP_FAILED = 6,
    };

    void MONITOR_API(uint32_t type) {
        Attr_API(ATTR + type, 1);
    }

    IliveMsgFactory(){}

    IliveMsgFactory(const IliveMsgFactory&);

    IliveMsgFactory& operator=(const IliveMsgFactory&);
};

} //namespace HANDLER

} //namespace TITANS


#endif //_TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVEMSGFATORY_H_
