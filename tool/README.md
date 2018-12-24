Titans 工具集
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
--param={\"add_method\":1, \"flow_log\":1}

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