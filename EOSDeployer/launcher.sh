#!/bin/bash

rm ~/eosio-wallet -rf
killall nodeos 
cd ~/tmp/eos/build
#booting eosio
eosio-launcher -p 4 -n 10 -s star -d 1 -f --nodeos "--verbose-http-errors --contracts-console"
bash bios_boot.sh
#create wallet
cleos --wallet-url http://localhost:8899 wallet create 2>&1 1>wallet.txt
#cleos --wallet-url http://localhost:8899 wallet unlock --password PW5KkcjxTPT56k6beua4ezhShwqbc2Kfsf9AW2rXZ5v4yv4Z46ihk
#import eosio.token key
cleos --wallet-url http://localhost:8899 wallet import 5K2cDnjdSF1RpNrxGuwAHvqwmNqRtSooFjbwLTVwMY2BJj4KWbw
#import system keys
cleos --wallet-url http://localhost:8899 wallet import 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
cleos --wallet-url http://localhost:8899 wallet import 5JB9z6pEobij5izCx46ECHUKP2BMobSiRBTPYn2j4MgzTQr5wdi
cleos --wallet-url http://localhost:8899 wallet import 5JZ9UL4ktmGjJ8fD1b3VmqKBCMaNENHkUd1RgikKpF2SbCVEsR1
cleos --wallet-url http://localhost:8899 wallet import 5JRkNnaSm5X6bHnkzibNH5wMTGjLNsebsnANkS4L9ZXZ8b7igYz
cleos --wallet-url http://localhost:8899 wallet import 5KU82BrcYmkincceyz95KMfvECCNxJEKPpcESas6BewXTfi6hPd
cleos --wallet-url http://localhost:8899 wallet import 5JkTTGCECNWwWJagt8vH2WEUnuegXPtwQTbqwJbP2tQGLqnndWE
cleos --wallet-url http://localhost:8899 wallet import 5JU9p7PtBoTPDzzT9GKLo2pxwn9qoCtvwRJ7VUSfnX5RNxzA2in
cleos --wallet-url http://localhost:8899 wallet import 5HsuCGTihp4YKxtVNz3x6X8LKqEUryyNfYTDpoFAGwHHrSn3xw1
cleos --wallet-url http://localhost:8899 wallet import 5JFHUx31x3aCeSuYkU5wrBu5UzQ3RsFE7a3sxyS4Lyv9wYyShYc
cleos --wallet-url http://localhost:8899 wallet import 5JgZJKhvj5Su9zdE1pK9pR7fHwmsfWwwQkV2hNH1XQjdXKX54Sg
cleos --wallet-url http://localhost:8899 wallet import 5KDTdBPxnJDLtkrD6KMJX79EAYnVkp351gKLtueXu9LNECzJcWn
#create eosio.token account
cleos --wallet-url http://localhost:8899 -u http://localhost:8788 create account eosio eosio.token EOS7SeFfxtRuzRAMiyCqi45b3mxkgf1DWMnnSBqZA1a93wxd3KQMJ -p eosio
#deploy eosio.token contract
cleos --wallet-url http://localhost:8899 -u http://localhost:8788 set contract eosio.token ~/tmp/eos/build/contracts/eosio.token -p eosio.token


