#ifndef _TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVEMSG_H_
#define _TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVEMSG_H_

#include <syncincl.h>
#include <Attr_API.h>
#include <proto/proto_ilive.h>
#include <task/RpcMgr.h>

#include <codec/ctx/CtxCodec.h>
#include <spp/handler/ctx/ilive_ctx/IliveContext.h>
#include <spp/handler/ctx/ilive_ctx/IliveLog.h>

namespace TITANS {

namespace HANDLER {

class IliveMsg :public SPP_SYNCFRAME::CSyncMsg{

public:

    IliveMsg() {
        ATTR = 0;
        m_logic_Attr = 0;
        m_retcode = 0;
        m_send_reply = false;
    }

    virtual ~IliveMsg() {}

    //@desc - 消息初始化
    //@param
    //  arg     - 初始化参数
    //@return
    //  int     - 0 成功
    virtual int Init(void* arg) {

        ATTR = *(static_cast<uint32_t*>(arg));
        return 0;
    }

    //@desc - 设置业务monitor
    //@param
    //  logic_attr  - 业务monitor
    //@return
    //  int     - 0 成功
    virtual int SetLogicAttr(int32_t logic_attr) {

        m_logic_Attr = logic_attr - MONITOR_SPAN_0_10;
        return 0;
    }


    virtual int HandleProcess() {

        int ret = 0;
        do {
            //包体解包
            ret = Decode();
            if(ret != 0) {
                MONITOR_API(MONITOR_DECODE_REQ_FAILED); //请求包错误
                LLOG(LOG_ERROR, "decode request failed|subcmd=%d|client_ip=%u|", HeadReq().subcmd(), HeadReq().client_ip());
                SetRetcode(ret);
                break;
            }
            //处理子命令字
            ret = Process();
            //处理子命令字上报
            ret = ProcessReport(ret);
            if(ret != 0) {
                SetRetcode(ret);
                break;
            }           
            //包体打包
            ret = Encode();
            if(ret != 0) {
                MONITOR_API(MONITOR_ENCODE_RSP_FAILED); // encode响应错误
                LLOG(LOG_ERROR, "encode request failed|subcmd=%d|", HeadReq().subcmd());
                SetRetcode(ret);
                break;
            }
        }while(0);
        //回包
        SendReply();
        //上报
        Monitor();
        return 0;
    }

    //@desc - 获取context
    //@param
    //@return
    //  IliveCtxPtr
    virtual IliveCtxPtr GetContext() { return m_pCtx; }

    //@desc - 设置context
    //@param
    //  ctx     - context
    //@return
    //  void
    virtual void SetContext(IliveCtxPtr ctx) { m_pCtx = ctx; }

    //@desc - rpc 装饰函数，先填充包头再rpc调用
    //@param
    //@return
    //  int
    template<typename RPC_REQ, typename RPC_RSP>
    int Call(TASK::CRpc& rpc, RPC_REQ& req, RPC_RSP& rsp, std::string& err_msg, iLiveRequest* head_req, iLiveResponse* head_rsp) {

        TITANS::CODEC::EncodeHead(head_req, &HeadReq());
        return rpc.Call(req, rsp, err_msg, head_req, head_rsp);
    }

    //@desc - rpc 装饰函数，先填充包头再rpc调用
    //@param
    //@return
    //  int
    template<typename RPC_REQ, typename RPC_RSP>
    int Call(TASK::CRpc& rpc, RPC_REQ& req, RPC_RSP& rsp, std::string& err_msg) {

        iLiveRequest head_req;
        iLiveResponse head_rsp;
        head_req.CopyFrom(HeadReq());
        return rpc.Call(req, rsp, err_msg, &head_req, &head_rsp);
    }

protected:
    //@desc - 包体解包，失败直接回包，返回码写入包头
    //@param
    //@return
    // int  - 0 成功
    virtual int Decode() {
        return _pBodyCodec->ReqPkgDecode(HeadReq().ex().c_str(), HeadReq().ex().size(), NULL);
    }

    //@desc - 包体打包，失败直接回包，返回码写入包头
    //@param
    //@return
    // int  - 0 成功
    virtual int Encode() { 
        return _pBodyCodec->RspPkgEncode(*(HeadRsp().mutable_ex()));
    }

    //@desc - 消息业务处理函数
    //@param
    //@return
    // int  - 0 成功
    virtual int Process() { return 0; }

    //@desc - 业务处理结果上报， 失败直接回包，返回码写入包头
    //@param
    //  result  - Process 返回码
    //@return
    // int  - 0 成功
    virtual int ProcessReport(int result) { return 0; }

    //@desc - 获取请求包头
    //@param
    //@return
    // iLiveRequest&  - 请求包头
    virtual iLiveRequest& HeadReq() { return m_pCtx->HeadReq(); }

    //@desc - 获取回包包头
    //@param
    //  result  - Process 返回码
    //@return
    // iLiveResponse&  - 0 回包包头
    virtual iLiveResponse& HeadRsp() { return m_pCtx->HeadRsp(); }

    //@desc - 设置回包返回码
    //@param
    //  retcode  - 包头返回码
    //@return
    // void
    void SetRetcode( int retcode) { m_retcode = retcode; }

    //@desc - 获取设置的回包返回码
    //@param
    //@return
    // int  - 设置的回包返回码
    int GetRetcode() { return m_retcode; }

    //@desc - 回包
    //@param
    //  mode    - 0 只能回一次包，后续回包操作无效
    //          - 1 后续可以继续回包 
    //@return
    // void
    void SendReply(uint32_t mode = 0) {

        if(m_send_reply == true) {
            return;
        }
        if(mode == 0) {
            m_send_reply = true;
        }

        m_pCtx->HeadRsp().set_err_code(m_retcode);
        TITANS::CODEC::EncodeHead(&m_pCtx->HeadRsp(), &m_pCtx->HeadReq());
        //回包
        std::string str_rsp;
        m_pCtx->RspPkgEncode(str_rsp);
        blob_type blob;
        blob.data = const_cast<char*>(str_rsp.data());
        blob.len = static_cast<int>(str_rsp.size());
        this->SendToClient(blob);
        return;
    }

    //@desc - 设置包体解析器
    //@param
    //  codec  - 包体解析器    
    //@return
    // void
    void SetBodyCodec(TITANS::CODEC::BaseCodec* codec) {

        _pBodyCodec = codec;
    }

    void Monitor() {

        if (0 == m_retcode) {
            MONITOR_API(MONITOR_OK); //总成功量
            LatencyReport();
        } else {
            MONITOR_API(MONITOR_FAILED);
        }   
    } 

    void MONITOR_API(uint32_t type) {
        Attr_API(ATTR + type, 1);
        if(type >= MONITOR_SPAN_0_10) {
            Attr_API(m_logic_Attr + type, 1);
        }
    }

    void LatencyReport() {

        int timespan = GetMsgCost();
        timespan < 10 ? MONITOR_API(MONITOR_SPAN_0_10) : //延时10ms以内
        timespan < 20 ? MONITOR_API(MONITOR_SPAN_10_20) ://延时10-20ms
        timespan < 50 ? MONITOR_API(MONITOR_SPAN_20_50) ://延时20-50ms
        timespan < 100 ? MONITOR_API(MONITOR_SPAN_50_100) ://延时50-100ms
        timespan < 200 ? MONITOR_API(MONITOR_SPAN_100_200) ://延时100-200ms
        timespan < 500 ? MONITOR_API(MONITOR_SPAN_200_500) ://延时200-500ms
        timespan < 1000 ? MONITOR_API(MONITOR_SPAN_500_1000) ://延时500-1000ms
        timespan < 2000 ? MONITOR_API(MONITOR_SPAN_1000_2000) ://延时1000-2000ms
        timespan < 5000 ? MONITOR_API(MONITOR_SPAN_2000_5000) ://延时2000-5000ms
        MONITOR_API(MONITOR_SPAN_MORE_5000);//延时大于5000ms
    }

    enum Attr_Type {   
        MONITOR_ALL = 0,
        MONITOR_OK  = 1,
        MONITOR_FAILED = 2,
        MONITOR_WRONG_BIG_CMD = 3,
        MONITOR_WRONG_SUBCMD = 4,
        MONITOR_DECODE_REQ_FAILED = 5,
        MONITOR_ENCODE_RSP_FAILED = 6,
        MONITOR_SPAN_0_10 = 7,
        MONITOR_SPAN_10_20 = 8,
        MONITOR_SPAN_20_50 = 9,
        MONITOR_SPAN_50_100 = 10,
        MONITOR_SPAN_100_200 = 11,
        MONITOR_SPAN_200_500 = 12,
        MONITOR_SPAN_500_1000 = 13,
        MONITOR_SPAN_1000_2000 = 14,
        MONITOR_SPAN_2000_5000 = 15,
        MONITOR_SPAN_MORE_5000 = 16,
    };
 
private:
    uint32_t ATTR;
    uint32_t m_logic_Attr;
    int m_retcode;
    bool m_send_reply;

    IliveCtxPtr m_pCtx;
    TITANS::CODEC::BaseCodec* _pBodyCodec;
};


#define MSG_CAT(file, line) file##line
#define REGIST(Type, Msg)    \
    TITANS::HANDLER::IliveMsg* Msg##CreateFunc() { \
        TITANS::HANDLER::IliveMsg* ptr_msg = new Msg(); \
	    return ptr_msg;	\
    }\
    static int MSG_CAT(R, __LINE__) = TITANS::HANDLER::IliveMsgFactory::Instance()->Regist(Type, Msg##CreateFunc);

} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVEMSG_H_
