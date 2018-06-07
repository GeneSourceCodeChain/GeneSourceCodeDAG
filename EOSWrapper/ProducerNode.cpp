#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "ProducerNode.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace boost::posix_time;

ProducerNode::ProducerNode(
	string name,boost::tuple<string,string> keypair,string json,
	vector<boost::tuple<string,unsigned short> > peers,
	string address, unsigned short portnum, unsigned short httpportnum,
	vector<string> plugins
):Node(address,httpportnum,name) {
	if(exists(config_dir / name)) throw runtime_error("the name has been taken, choose another one!");
	if(0 == peers.size()) throw logic_error("at least the genesis node's ip address should be provided!");
	create_directory(config_dir / name);
	//1)put the genesis created private key into local wallet
	wallet.import(get<1>(keypair));
	//2)setup genesis json
	std::ofstream out((config_dir / name / "genesis.json").string());
	out << json;
	out.close();
	//2)launch nodeos
	vector<string> arguments = nodeosargs;
	arguments.push_back("--producer-name");
	arguments.push_back(name);
	arguments.push_back("--signature-provider=" + get<0>(keypair) + "=KEY:" + get<1>(keypair));
	arguments.push_back("--http-server-address");
	arguments.push_back(address + ":" + lexical_cast<string>(httpportnum));
	arguments.push_back("--p2p-listen-endpoint");
	arguments.push_back(address + ":" + lexical_cast<string>(portnum));
	for(auto & peer : peers) {
		arguments.push_back("--p2p-peer-address");
		arguments.push_back(get<0>(peer) + ":" + lexical_cast<string>(get<1>(peer)));
	}
	for(auto & plugin : plugins) {
		arguments.push_back("--plugin");
		arguments.push_back(plugin);
	}
	arguments.push_back("--config-dir");
	arguments.push_back((config_dir / name).string());
	arguments.push_back("--data-dir");
	arguments.push_back((config_dir / name).string());
	arguments.push_back("--blocks-dir");
	arguments.push_back((config_dir / name / "blocks").string());
	arguments.push_back("--genesis-json");
	arguments.push_back((config_dir / name / "genesis.json").string());
#ifndef NDEBUG
	cout<<nodeoscmd<<" ";
	for(auto & arg : arguments) cout<<arg<<" ";
	cout<<endl;
#endif
	nodeos = boost::shared_ptr<child>(new child(nodeoscmd,arguments,std_err > buf,ios,ec));
	print_msg_handle = boost::shared_ptr<boost::thread>(new boost::thread(bind(&Node::print_message,this)));
	boost::this_thread::sleep(seconds(1));
}

ProducerNode::~ProducerNode()
{
}

bool ProducerNode::run()
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the producer nodeo is not running!");
	//nothing to do
	return true;
}

