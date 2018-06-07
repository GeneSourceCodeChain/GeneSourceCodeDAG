#ifndef PRODUCERNODE_H
#define PRODUCERNODE_H

#include <string>
#include "Node.h"

using namespace std;

class ProducerNode : public Node {
public:
	ProducerNode(
		string name,boost::tuple<string,string> keypair,string json,
		vector<boost::tuple<string,unsigned short> > peers,
		string address = "localhost", unsigned short portnum = 8888, unsigned short httpportnum = 7888,
		vector<string> plugins = {
			"eosio::producer_plugin",
			"eosio::chain_api_plugin",
			"eosio::http_plugin",
			"eosio::history_api_plugin",
			"eosio::net_plugin",
			"eosio::net_api_plugin"
		}
	);
	virtual ~ProducerNode();
	virtual bool run();
};

#endif

