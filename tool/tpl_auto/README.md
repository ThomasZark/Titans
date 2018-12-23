# Titans 自动化生成工具

tpl_auto 通过对proto协议的解析，并使用ctemplate来实现代码自动化生成

##模板变量

**全局** 

| 参数 | 描述 | 
|:---:|:---:|
| SRV_NAME | service 名称 | 
| UP_SRV_NAME | service 大写名称 | 
| CAMEL_SRV_NAME | service 驼峰命名(去除_) | 
| PACKAGE | 协议包名 | 
| PROTO | 协议名称(无后缀) | 
| BIG_CMD | 大命令字 | 

**二级列表变量** 

| 参数 | 描述 | 
|:---:|:---:|
| TMEM | tmem | 
| HIPPO | hippo | 
| HIPPO_SENDER | hippo sender | 
| TDBANK | tdbank | 
| FLOW_LOG | flow_log |
| PROC_FUNC | rpc 函数列表 | 

**PROC_FUNC** 

| 参数 | 描述 | 
|:---:|:---:|
| SRV_NAME | service 名称 | 
| CAMEL_SRV_NAME | service 驼峰命名(去除_) | 
| PACKAGE | 协议包名 | 
| PROTO | 协议名称(无后缀) | 
| BIG_CMD | 大命令字 | 
| SUB_CMD | 子命令字 | 