# EOS testnet deployment tools

this project implements two scripts for EOS testnet deployment.

### introduction

EOS network deployment is more complex than that of ETH. we provide two scripts which can deploy EOS testnet in one command. before try the scripts, you need to make sure your EOS source code directory is placed under ~/tmp or you need to modify the script a little according to your configuration.

1. deploy with eosio-launcher

launch a simple testnet in which any manipulation is not contrained by how many sys coin you can stake.

```bash
bash launcher.sh
```

2. deploy with bios-boot-tutorial.py

launch a full functional testnet with all mechanism as mainnet.

```bash
bash pylauncher.sh
```

