# Titans 

Titans是一个面向后台的rpc开发框架，旨在提高后台逻辑服务开发效率。

## 特点

## 目录
- core， Titans核心部件，提供了Titans核心接口
- codec，Titans 协议解析器，自定义协议实现标准打包解包接口


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

message HelloRsp {
    optional int32 retcode = 1;//
    optional string str = 2;//
}

service TestSvr {
    rpc Echo(EchoReq) returns (EchoRsp);//
    rpc Hello(HelloReq) returns (HelloRsp);//
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