#!/bin/bash

killall keosd
rm ~/.nodes -rf
rm ~/eosio-wallet -rf
cd ~/tmp/eos/tutorials/bios-boot-tutorial/
python3 bios-boot-tutorial.py -a --user-limit 10 --producer-limit 4 --num-producers-vote 2

