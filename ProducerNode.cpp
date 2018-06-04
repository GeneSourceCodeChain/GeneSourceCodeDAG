#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include "ProducerNode.h"

using namespace boost;
using namespace boost::filesystem;

ProducerNode::ProducerNode(
	string name,boost::tuple<string,string> keypair,
	vector<boost::tuple<string,unsigned short> > peers,
	string address, unsigned short portnum, unsigned short httpportnum,
	vector<string> plugins
):Node(address,portnum,name) {
	if(exists(config_dir / name)) throw runtime_error("the name has been taken, choose another one!");
	if(0 == peers.size()) throw logic_error("at least the genesis node's ip address should be provided!");
	create_directory(config_dir / name);
	//1)put the genesis created private key into local wallet
	wallet.import(get<1>(keypair));
	//2)launch nodeos
	vector<string> arguments = nodeosargs;
	arguments.push_back("-e");
	arguments.push_back("--private-key '[\"" + get<0>(keypair) + "\",\"" + get<1>(keypair) + "\"]'");
	arguments.push_back("--http-server-address " + address + ":" + lexical_cast<string>(httpportnum));
	for(auto & peer : peers)
		arguments.push_back("--p2p-peer-address " + get<0>(peer) + ":" + lexical_cast<string>(get<1>(peer)));
	for(auto & plugin : plugins)
		arguments.push_back("--plugin " + plugin);
	arguments.push_back("--config-dir " + (config_dir / name).string());
	arguments.push_back("--data-dir " + (config_dir / name).string());
	arguments.push_back("--blocks-dir " + (config_dir / name / "blocks").string());
	nodeos = boost::shared_ptr<child>(new child(nodeoscmd,arguments,std_out > nodeos_out));
	print_msg_handle = boost::shared_ptr<boost::thread>(new boost::thread(bind(&Node::print_message,this)));
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
