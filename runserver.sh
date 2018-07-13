#!/bin/bash
#reset
rm -rf data/*
#create genesis block
geth --identity "MyNodeName" --rpc --rpcport "8080" --rpccorsdomain "*" --datadir data --keystore keystore --rpcaddr "192.168.1.100" --wsaddr "192.168.1.100" --port "30303" --nodiscover --rpcapi "db,eth,net,web3" --networkid 1999 init genesis.json
#start the genesis node
geth --identity "MyNodeName" --rpc --rpcport "8080" --rpccorsdomain "*" --datadir data --keystore keystore --rpcaddr "192.168.1.100" --wsaddr "192.168.1.100" --port "30303" --nodiscover --rpcapi "db,eth,net,web3" --networkid 1999 console

