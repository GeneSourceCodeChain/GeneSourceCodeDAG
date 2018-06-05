#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "GenesisNode.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace boost::posix_time;

GenesisNode::GenesisNode(
	string name,
	vector<boost::tuple<string,unsigned short> > peers,
	string address,unsigned short httpportnum,
	vector<string> plugins
):Node(address,httpportnum,name) {
	if(exists(config_dir / name)) throw runtime_error("the name has been taken, choose another one!");
	create_directory(config_dir / name);
	//1)create a private key for eosio account and import into wallet
	boost::tuple<bool,string,string> eosio_keypair = create_keypair();
	if(false == get<0>(eosio_keypair)) throw runtime_error("failed to create keypair");
	wallet.import(get<2>(eosio_keypair));
	//2)setup minimum necessary arguments
	vector<string> arguments(nodeosargs.begin(),nodeosargs.end());
	arguments.push_back("-e");
	arguments.push_back("--producer-name");
	arguments.push_back(name);
	//we use self created keypair as keys for eosio account
	arguments.push_back("--private-key");
	arguments.push_back("'[\"" + get<1>(eosio_keypair) + "\",\"" + get<2>(eosio_keypair) + "\"]'");
	arguments.push_back("--http-server-address");
	arguments.push_back(address + ":" + lexical_cast<string>(httpportnum));
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
	//3)launch nodeos
#ifndef NDEBUG
	cout<<nodeoscmd<<" ";
	for(auto & arg : arguments) cout<<arg<<" ";
	cout<<endl;
#endif
	nodeos = boost::shared_ptr<child>(new child(nodeoscmd,arguments,std_err > buf,ios,ec));
	print_msg_handle = boost::shared_ptr<boost::thread>(new boost::thread(bind(&Node::print_message,this)));
	boost::this_thread::sleep(seconds(1));
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
	retval = set_contract("eosio.token","./contracts/eosio.token");
	retval = set_contract("eosio.msig","./contracts/eosio.msig");
	retval = call_contract("eosio.token","create","{\"issuer\":\"eosio\",\"maximum_supply\":\"2000000000.0000 EOS\"}","eosio.token");
	retval = call_contract("eosio.token","issue","{\"to\":\"eosio\",\"quantity\":\"2000000000.0000 EOS\",\"memo\":\"issue all system coin to eosio\"}","eosio");
	retval = set_contract("eosio","./contracts/eosio.system");
	retval = call_contract("eosio","setpriv","[\"eosio.msig\",1]","eosio@active");
	return true;
}

bool GenesisNode::create_account(string accountname,string publickey,string creater)
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos is not running!");
	vector<string> arguments(cleosargs.begin(),cleosargs.end());
	arguments.push_back("create");
	arguments.push_back("account");
	arguments.push_back(creater);
	arguments.push_back(accountname);
	arguments.push_back(publickey);
	io_service ios;
	future<string> buf;
	std::error_code ec;
#ifndef NDEBUG
	cout<<cleoscmd<<" ";
	for(auto & arg: arguments) cout<<arg<<" ";
	cout<<endl;
#endif
	child c(cleoscmd,arguments,std_out > buf,ios,ec);
	ios.run();
	return (EXIT_SUCCESS == ec.value())?true:false;
}

