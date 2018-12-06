# Titans 

Titans是一个基于spp开发的面向后台的rpc开发框架，旨在提高后台逻辑服务开发效率。

## 特点

## 目录
- core， Titans核心部件，提供了Titans核心接口
- codec，Titans 协议，自定义协议实现标准打包解包接口
- spp， Titans 提供基于spp的具体实现


## 快速开始

项目结构
```shell
TestSvr
├── build
├── conf
│   └── TestSvr.conf
├── README.md
└── src
    ├── msg
    │   ├── TestSvr.cpp
    │   ├── SubMethod1Msg.cpp
    │   └── SubMethod2Msg.cpp
    └── SvrInit.cpp
```

SvrInit.cpp
```shell
SppServer* GetServer() {

    static SppServer sppServerImpl;
    static MtHandler<IliveCodec, TestMsg> mt_handler;
    
    //绑定处理器，设置服务名称
    sppServerImpl.SetHandler(&mt_handler)->SetServerName("TestSvr");
    return &sppServerImpl;
}

//注册运行时服务
//spp框架提供业务接口运行时回调
//不同框架启动方式不同
REGIST_SPPSERVER(GetServer())
```