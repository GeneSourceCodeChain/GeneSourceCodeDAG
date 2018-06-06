#ifndef GENESISNODE_H
#define GENESISNODE_H

#include <string>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include "Node.h"
#include "Wallet.h"

using namespace std;
using namespace boost;

class GenesisNode : public Node {
public:
	GenesisNode(
		string name,
		vector<boost::tuple<string,unsigned short> > peers,
		string address = "0.0.0.0",unsigned short portnum = 9876,unsigned short httpportnum = 8888,
		vector<string> plugins = {
			"eosio::producer_plugin",
			"eosio::chain_api_plugin",
			"eosio::http_plugin",
			"eosio::history_api_plugin",
			"eosio::net_plugin"
		}
	);
	virtual ~GenesisNode();
	virtual bool run();
protected:
	bool create_account_without_smartcontract(string accountname,string publickey,string creater);
};

#endif

