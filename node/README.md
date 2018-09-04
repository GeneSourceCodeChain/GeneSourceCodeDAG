# steps to launch candidate block producer node

1. edit config/config.ini and do the following modifications

set up variables related to json rpc server, http-server-address = \<your own ip\>:\<any available port\>

set up variables related to p2p server, p2p-listen-endpoint = \<your own ip\>:\<any available port\>

set up the following variable with your own account, producer-name = \<account（contact breadbread1984 for registration）\>

set up the following variable with your own public and private key, signature-provider = \<public key\>=KEY:\<private key\>

2. launch node service

run script runnodeos.sh

3. launch local wallet service

run script runkeosd.sh

4. usage

```bash
cleos --wallet-url http://localhost:6666 -u http://<ip>:<port>  <eosio cmd>
```
