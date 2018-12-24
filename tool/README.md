Titans 工具集
===
## attr_incr
**monitor生成工具** 

| 参数 | 描述 |
|:---:|:---:|:---:|
| cur_file | attr_define.h文件路径 |
| desc_file | attr.desc文件路径 |

扫描项目下的monitor属性，增量生成并添加到cur_file文件中，同时生成desc_file文件，供批量申请属性
支持批量申请rpc，子命令字延时统计。

rpc 批量申请
```shell
//server
CDemoInit::Init(,,,MONITOR_DEMO,);
```
子命令字延时统计
```shell
//LogicMonitor
SetLogicAttr(MONITOR_DEMO);
```
**usage**:attr_incr --cur_file=./common/attr_define.h --desc_file=./common/attr.desc

## setting_scan
**setting扫描工具**

| 参数 | 描述 |
|:---:|:---:|:---:|
| setting_h | setting.h文件路径 |
| setting_cpp | setting.cpp文件路径 |
| prefix | 扫描前缀 |

扫描项目下面的setting配置， 增量添加到setting_h 跟 setting_cpp文件中
配置向有命名要求，以特定字母开头紧接大写字母，eg uiNum

| 开头 | 含义 |
|:---:|:---:|:---:|
| i | int |
| ui |uint32_t |
| b | bool |
| str | std::string |
| ul | unsigned long |
| ull | unsigned long long |
**usage**:setting_scan --setting_cpp=./common/setting.cpp --setting_h=./common/setting.h --prefix="INS\\(sSettingPlugin\\)->"

## rpc_incr
**rpc 扫描工具**

| 参数 | 描述 |
|:---:|:---:|:---:|
| cur_file | attr_define.h文件路径 |
| desc_file | attr.desc文件路径 |
| conf_file | conf 配置文件路径 |
| rpc_path | rpc文件路径 |

扫描rpc_path下面的rpc文件，增量添加rpcmonitor到cur_file中，更新desc_file文件供批量申请monitor，增量添加对应的rpc配置到conf_file文件中
**usage** rpc_incr --cur_file=./src/common/attr_define.h --desc_file=./src/common/attr.desc --rpc_path=./build/rpc --conf_file=./conf/svr.conf

## ctx_tpl_auto
**server 模版工具**

| 参数 | 描述 |
|:---:|:---:|:---:|
| proto_dir | proto文件路径 |
| proto_file | proto文件 |
| target_path | 生成路径 |
| tpl_path | 模版路径 |
| param | 其他参数，json格式 |
| method | 增加的子命令字函数名 |

解析proto协议，根据模版文件生成服务代码模版
**usage** ctx_tmpl_auto --proto_dir=`pwd` --target_path=`pwd` --tpl_path=/data/titans/tool/template/ctx_tpl_v3/ --proto_file=EchoSvr.proto
--param={\"add_method\":1, \"tdbank\":1, \"flow_log\":1}

## ctx_rpc_auto
**rpc 模版工具**

| 参数 | 描述 |
|:---:|:---:|:---:|
| proto_dir | proto文件路径 |
| proto_file | proto文件 |
| target_path | 生成路径 |
| tpl_path | 模版路径 |
| strict | 1不检查proto格式，0：严格检查 |

解析proto协议，根据模版文件生成rpc模版
**usage** ctx_rpc_auto --proto_dir=`pwd` --target_path=`pwd` --tpl_path=/data/titans/tool/template/ctx_tpl_v3/ --proto_file=EchoSvr.proto -strict=1

## 快速开始
依赖
jsoncpp
protobuf
glog
gflags
ctemplate

```shell
mkdir build -p
cd ./build
cmake ../ -DCMAKE_INSTALL_PREFIX=
make install
```