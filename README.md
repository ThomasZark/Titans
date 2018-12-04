# Titans 

Titans是一个基于spp开发的面向后台的rpc开发框架，旨在提高后台逻辑服务开发效率。

## 特点

## 目录
- core， Titans核心部件，提供了Titans核心接口
- core/server，Titans 服务标准接口，提供了默认spp实现
- core/handler， Titans 请求处理标准接口
- core/plugin，Titans 适配插件标准接口
- plugin， 第三方插件适配实现

## 快速开始

项目结构
```shell
IliveTmplSvr
├── build
├── conf
│   └── IliveTmplSvr.conf
├── README.md
└── src
    ├── msg
    │   ├── IliveTmplSvrMsg.cpp
    │   ├── SubMethod1Msg.cpp
    │   └── SubMethod2Msg.cpp
    └── IliveTmplSvr.cpp
```

IliveTmplSvr.cpp
```shell
SppServr serverImpl;
serverImpl.SetHandler(IliveHandler)->SetSvrName("IliveTmplSvr");
sSppServerMgr::Instance().SetSppServerImpl(&serverImpl);
```