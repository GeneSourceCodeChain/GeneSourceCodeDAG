# EOSWrapper
this project implements a wrapper for EOS.IO toolkits. building decentralized computation platform with this wrapper is really handy than typing commands.

### how to build
type the following command to build everything
```Bash
make
```

### how to test
compile EOSIO and place contract directory under current directory.

Then, build a testnet with the following command
```Bash
./demo
```

### FAQ
1. Error 3040005: Expired Transaction

This error occurs when your docker's time is behind real time. Try to check out and set time with date command.
