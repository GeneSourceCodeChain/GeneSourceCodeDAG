启动主节点流程
1）编辑config/config.ini文件，做如下修改
设置json rpc服务变量http-server-address = <ip>:<port>
设置p2p服务变量p2p-listen-endpoint = <ip>:<port>
用自己注册的账户名设置变量producer-name = <account>
用自己公钥私钥设置变量signature-provider = <公钥>=KEY:<私钥>
2）启动节点服务
运行runnodeos.sh脚本。
3）启动本地钱包服务
运行脚本runkeosd.sh脚本
4）使用方法
cleos --wallet-url http://localhost:6666 -u http://<ip>:<port>  <eosio cmd>
