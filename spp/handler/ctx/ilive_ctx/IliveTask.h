#ifndef _TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVETEAK_H_
#define _TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVETEAK_H_

#include <micro_thread.h>
#include <proto/proto_ilive.h>

#include <codec/ctx/CtxCodec.h>
#include <spp/handler/ctx/ilive_ctx/IliveMsg.h>

namespace TITANS {

namespace HANDLER {

//@desc - IliveTask
//@template param
//  REQ  - 请求包体类型
//  RSP  - 回包包体类型
template<typename REQ, typename RSP>
class IliveTask:public IMtTask {

public:
    IliveTask(TASK::CRpc& rpc)
    :m_rpc(rpc) {}

    virtual ~IliveTask() {}

    //@desc - 获取请求包体
    //@param
    //@return
    // REQ&   - 请求包体
    REQ& BodyReq() { return m_body_req; }
    
    //@desc - 获取回包包体
    //@param
    //@return
    // RSP&   - 回包包体
    RSP& BodyRsp() { return m_body_rsp; }

    //@desc - 获取请求包头
    //@param
    //@return
    // iLiveRequest&   - 请求包头
    iLiveRequest& HeadReq() { return m_head_req; }

    //@desc - 获取回包包头
    //@param
    //@return
    // iLiveResponse&   - 回包包头
    iLiveResponse& HeadRsp() { return m_head_rsp; }

    //@desc - 获取错误信息
    //@param
    //@return
    // string&   - 错误信息
    std::string& ErrMsg() { return m_err_msg; }

    //@desc - 获取 Process结果
    //@param
    //@return
    // int   - Process结果
    virtual int Finish() { return GetResult(); }

    //@desc - task 逻辑处理
    //@param
    //@return
    // int   - 逻辑处理结果
    virtual int TaskProcess() {

        return m_rpc.Call(m_body_req, m_body_rsp, m_err_msg, &m_head_req, &m_head_rsp);
    }

    virtual int Process() {
        MicroThread* thread = MtFrame::Instance()->GetRootThread();
        if (thread) {
            IliveMsg* msg = NULL;
            msg = dynamic_cast<IliveMsg*>(static_cast<CSyncMsg*>(thread->GetThreadArgs()));
            if (msg) {
                IliveCtxPtr pCtx = msg->GetContext();
                TITANS::CODEC::EncodeHead(&m_head_req, &pCtx->HeadReq());
            }
        }
        return TaskProcess();
    }

protected:
    REQ m_body_req;
    RSP m_body_rsp;
    iLiveRequest m_head_req;
    iLiveResponse m_head_rsp;
    std::string m_err_msg;
    TASK::CRpc& m_rpc;
};

} //namespace HANDLER

} //namespace TITANS

#endif //_TITANS_SPP_HANDLER_CTX_ILIVE_CTX_ILIVETEAK_H_