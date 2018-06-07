#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "GenesisNode.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace boost::posix_time;
using namespace boost::property_tree;

GenesisNode::GenesisNode(
	string name,
	vector<boost::tuple<string,unsigned short> > peers,
	string address,unsigned short portnum,unsigned short httpportnum,
	vector<string> plugins
):Node(address,httpportnum,name) {
	if(exists(config_dir / name)) throw runtime_error("the name has been taken, choose another one!");
	create_directory(config_dir / name);
	//0)create a private key for eosio account and import into wallet
	boost::tuple<bool,string,string> eosio_keypair = create_keypair();
	if(false == get<0>(eosio_keypair)) throw runtime_error("failed to create keypair");
	wallet.import(get<2>(eosio_keypair));
	//1)setup genesis json
	initial_key = get<1>(eosio_keypair);
	ptime time = second_clock::local_time();
	time_facet * facet = new time_facet();
	facet->format("%Y-%m-%dT%H:%M:%S");
	stringstream sstr;
	sstr.imbue(locale(locale::classic(),facet));
	sstr<<time;
	sstr>>initial_timestamp;
	string json = get_configure();
	std::ofstream out((config_dir / name / "genesis.json").string());
	out<<json;
	out.close();
	//2)setup minimum necessary arguments
	vector<string> arguments(nodeosargs.begin(),nodeosargs.end());
	arguments.push_back("--enable-stale-production");
	arguments.push_back("--producer-name");
	arguments.push_back(name);
	//we use self created keypair as keys for eosio account
	arguments.push_back("--signature-provider=" + get<1>(eosio_keypair) + "=KEY:" + get<2>(eosio_keypair));
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
	arguments.push_back("--genesis-json");
	arguments.push_back((config_dir / name / "genesis.json").string());
	//3)launch nodeos
#ifndef NDEBUG
	cout<<nodeoscmd<<" ";
	for(auto & arg : arguments) cout<<arg<<" ";
	cout<<endl;
#endif
	nodeos = boost::shared_ptr<child>(new child(nodeoscmd,arguments,std_out > buf,ios,ec));
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
	retval = create_account_without_smartcontract("eosio.bpay",get<1>(sys_keypair),"eosio");
	retval = create_account_without_smartcontract("eosio.msig",get<1>(sys_keypair),"eosio");
	retval = create_account_without_smartcontract("eosio.names",get<1>(sys_keypair),"eosio");
	retval = create_account_without_smartcontract("eosio.ram",get<1>(sys_keypair),"eosio");
	retval = create_account_without_smartcontract("eosio.ramfee",get<1>(sys_keypair),"eosio");
	retval = create_account_without_smartcontract("eosio.saving",get<1>(sys_keypair),"eosio");
	retval = create_account_without_smartcontract("eosio.stake",get<1>(sys_keypair),"eosio");
	retval = create_account_without_smartcontract("eosio.token",get<1>(sys_keypair),"eosio");
	retval = create_account_without_smartcontract("eosio.vpay",get<1>(sys_keypair),"eosio");
	//2)setup system contracts
	retval = set_contract("eosio.token","./contracts/eosio.token");
	retval = set_contract("eosio.msig","./contracts/eosio.msig");
	retval = call_contract("eosio.token","create","{\"issuer\":\"eosio\",\"maximum_supply\":\"2000000000.0000 SYS\"}","eosio.token");
	retval = call_contract("eosio.token","issue","{\"to\":\"eosio\",\"quantity\":\"2000000000.0000 SYS\",\"memo\":\"issue all system coin to eosio\"}","eosio");
	retval = set_contract("eosio","./contracts/eosio.system");
	retval = call_contract("eosio","setpriv","[\"eosio.msig\",1]","eosio@active");
	return true;
}

string GenesisNode::get_configure()
{
	ptree json;
	json.put("initial_timestamp",initial_timestamp);
	json.put("initial_key",initial_key);
	json.put("initial_chain_id","0000000000000000000000000000000000000000000000000000000000000000");
	ptree configure;
	configure.put("max_block_net_usage",1048576);
	configure.put("target_block_net_usage_pct",1000);
	configure.put("max_transaction_net_usage",524288);
	configure.put("base_per_transaction_net_usage",12);
	configure.put("net_usage_leeway",500);
	configure.put("context_free_discount_net_usage_num",20);
	configure.put("context_free_discount_net_usage_den",100);
	configure.put("max_block_cpu_usage",100000);
	configure.put("target_block_cpu_usage_pct",500);
	configure.put("max_transaction_cpu_usage",50000);
	configure.put("min_transaction_cpu_usage",100);
	configure.put("max_transaction_lifetime",3600);
	configure.put("deferred_trx_expiration_window",600);
	configure.put("max_transaction_delay",3888000);
	configure.put("max_inline_action_size",4096);
	configure.put("max_inline_action_depth",4);
	configure.put("max_authority_depth",6);
	configure.put("max_generated_transaction_count",16);
	json.add_child("initial_configuration",configure);
	stringstream retval;
	write_json(retval,json);
	return retval.str();
}

bool GenesisNode::create_account_without_smartcontract(string accountname,string publickey,string creater)
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

