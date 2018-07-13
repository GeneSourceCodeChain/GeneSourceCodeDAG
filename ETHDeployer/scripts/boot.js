/*unlock wallets*/
personal.unlockAccount(eth.accounts[0],'genesourceco');
personal.unlockAccount(eth.accounts[1],'player11111a');
personal.unlockAccount(eth.accounts[2],'player11111b');
personal.unlockAccount(eth.accounts[3],'landloveryes');
/*set miners*/
miner.setEtherbase(eth.accounts[0]);
miner.setEtherbase(eth.accounts[1]);
miner.setEtherbase(eth.accounts[2]);
miner.setEtherbase(eth.accounts[3]);
/*start mining*/
miner.start();
