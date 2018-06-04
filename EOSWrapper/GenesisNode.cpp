#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include "GenesisNode.h"

using namespace boost;
using namespace boost::filesystem;

GenesisNode::GenesisNode(
	string name,
	vector<boost::tuple<string,unsigned short> > peers,
	string address,unsigned short portnum,
	vector<string> plugins
):Node(address,portnum,name) {
	if(exists(config_dir / name)) throw runtime_error("the name has been taken, choose another one!");
	create_directory(config_dir / name);
	//1)create a private key for eosio account and import into wallet
	boost::tuple<bool,string,string> eosio_keypair = create_keypair();
	if(false == get<0>(eosio_keypair)) throw runtime_error("failed to create keypair");
	wallet.import(get<2>(eosio_keypair));
	//2)setup minimum necessary arguments
	vector<string> arguments = nodeosargs;
	arguments.push_back("-e");
	arguments.push_back("-p eosio");
	//we use self created keypair as keys for eosio account
	arguments.push_back("--private-key '[\"" + get<1>(eosio_keypair) + "\",\"" + get<2>(eosio_keypair) + "\"]'");
	arguments.push_back("--http-server-address " + address + ":" + lexical_cast<string>(portnum));
	for(auto & peer : peers)
		arguments.push_back("--p2p-peer-address " + get<0>(peer) + ":" + lexical_cast<string>(get<1>(peer)));
	for(auto & plugin : plugins)
		arguments.push_back("--plugin " + plugin);
	arguments.push_back("--config-dir " + (config_dir / name).string());
	arguments.push_back("--data-dir " + (config_dir / name).string());
	//3)launch nodeos
	nodeos = boost::shared_ptr<child>(new child(nodeoscmd,arguments,std_out > nodeos_out));
	print_msg_handle = boost::shared_ptr<boost::thread>(new boost::thread(bind(&Node::print_message,this)));
}

GenesisNode::~GenesisNode()
{
}

bool GenesisNode::run()
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the genesis node is not running!");
	//1)create important system accounts
	boost::tuple<bool,string,string> sys_keypair = create_keypair();
	if(false == get<0>(sys_keypair)) throw runtime_error("failed to create key pair!");
	bool retval;
	retval = create_account("eosio.bpay",get<1>(sys_keypair),"eosio");
	retval = create_account("eosio.msig",get<1>(sys_keypair),"eosio");
	retval = create_account("eosio.names",get<1>(sys_keypair),"eosio");
	retval = create_account("eosio.ram",get<1>(sys_keypair),"eosio");
	retval = create_account("eosio.ramfee",get<1>(sys_keypair),"eosio");
	retval = create_account("eosio.saving",get<1>(sys_keypair),"eosio");
	retval = create_account("eosio.stake",get<1>(sys_keypair),"eosio");
	retval = create_account("eosio.token",get<1>(sys_keypair),"eosio");
	retval = create_account("eosio.vpay",get<1>(sys_keypair),"eosio");
	//2)setup system contracts
	retval = set_contract("eosio.token","/contracts/eosio.token");
	retval = set_contract("eosio.msig","/contracts/eosio.msig");
	retval = call_contract("eosio.token","create","'[\"eosio\",\"2000000000.0000 SYS\"]'","eosio.token");
	retval = call_contract("eosio.token","issue","'[\"eosio\",\"2000000000.0000 SYS\",\"memo\"]'","eosio");
	retval = set_contract("eosio","/contracts/eosio.system");
	retval = call_contract("eosio","setpriv","'[\"eosio.msig\",1]'","eosio@active");
	return true;
}

bool GenesisNode::create_account(string accountname,string publickey,string creater)
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos is not running!");
	vector<string> arguments = cleosargs;
	arguments.push_back("create account");
	arguments.push_back(creater);
	arguments.push_back(accountname);
	arguments.push_back(publickey);
	child c(cleoscmd,arguments);
	c.wait();
	return (EXIT_SUCCESS == c.exit_code())?true:false;
}

