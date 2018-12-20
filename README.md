# Titans 

Titans是一个基于spp开发的面向后台的rpc开发框架，旨在提高后台逻辑服务开发效率。

## 特点

## 目录
- core， Titans核心部件，提供了Titans核心接口
- codec，Titans 协议解析器，自定义协议实现标准打包解包接口
- spp， Titans 提供基于spp的具体实现


## 快速开始
设计协议文件
```shell
package TestSvr;

enum BIG_CMD {
    TEST_SVR = 0x7777;//
};

enum SUB_CMD {
    ECHO  = 0x1; //
    HELLO = 0x2; //
};

message EchoReq {
    optional string param = 1;//
}

message EchoRsp {
    optional int32 retcode = 1;//
}

message HelloReq {
    optional string param = 1;//
}

message HelloReq {
    optional int32 retcode = 1;//
    optional string str = 2;//
}

service TestSvr {
    rpc Echo(EchoReq) returns (EchoRsp);//
    rcp Hello(HelloReq) returns (HelloRsp);//
}

```
执行工具生成项目，结构如下
```shell
TestSvr
├── README.md
├── build
├── conf                            //配置文件
├── proto                           //协议文件
|   ├── TestSvr.proto
└── src
    ├── common                      //公共目录
    ├── plugin                      //插件目录
    ├── msg
    │   ├── TestSvrMsg.h
    │   ├── EchoMsg.cpp
    │   └── HelloMsg.cpp
    └── SvrInit.cpp
    └── CMakeLists.txt
```
ECHO请求处理实现，EchoMsg.cpp
```shell
class EchoMsg: public SppTmplSvrMsg<EchoReq, EchoRsp> {

public:
    EchoMsg() {};

    virtual ~EchoMsg() {};

    virtual int Process();

};
//注册命令子ECHO的处理类
REGIST(ECHO, EchoMsg)

//具体业务实现接口
int EchoMsg::Process() {

    //echo 请求包
    LLOG(LOG_DEBUG, "req=%s", BodyReq().ShortDebugString().c_str());
    BodyRsp().set_retcode(0);
    return 0;
}
```
</br>
HELLO请求处理实现，HelloMsg.cpp
```shell
class HelloMsg: public SppTmplSvrMsg<HelloReq, HelloRsp> {

public:
    HelloMsg() {};

    virtual ~HelloMsg() {};

    virtual int Process();
};
//注册命令子HELLO的处理类
REGIST(HELLO, HelloMsg)

//具体业务实现接口
int HelloMsg::Process() {

    BodyRsp().set_retcode(0);
    BodyRsp().set_str("Hello " + BodyReq().param());
    return 0;
}
```
SvrInit.cpp
```shell
SppServer* GetServer() {

    static SppServer sppServerImpl;
    //声明ilive协议协程版处理器
    static MtCtxHandler<IliveCodec, IliveMsg, IliveMsgFactory> mt_ctx_handler(TestSvr::TEST_SVR, 0, 2000);
    
    sppServerImpl.SetHandler(&mt_ctx_handler)->SetServerName("TestSvr");
    return &sppServerImpl;
}

//注册运行时服务
//spp框架提供业务接口运行时回调
//不同框架启动方式不同
REGIST_SPPSERVER(GetServer())
```