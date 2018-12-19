#include <spp/handler/ctx/ilive_ctx/IliveMsg.h>
#include <spp/handler/ctx/ilive_ctx/IliveMsgFactory.h>

using namespace TITANS::HANDLER;

class IliveErrorMsg: public IliveMsg {

public:
    IliveErrorMsg(){}

    virtual ~IliveErrorMsg() {}

    virtual int HandleProcess() {

        SendReply();
        return 0;
    }

    virtual int Init(void* arg) {

        SetRetcode(*(static_cast<int*>(arg)));
        return 0;
    }
};

REGIST(0, IliveErrorMsg)