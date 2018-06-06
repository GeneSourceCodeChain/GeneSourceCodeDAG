#include <stdexcept>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "Node.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace boost::algorithm;

const path Node::config_dir = CONFIGDIR;
string Node::initial_key = "";
string Node::initial_timestamp = "";

Node::Node()
{
	if(false == exists(config_dir)) create_directory(config_dir);
}

Node::Node(string add,unsigned short pn,string name)
:wallet(add,pn),walletname(name),
nodeoscmd(search_path("nodeos").string()),nodeosargs({}),
cleoscmd(search_path("cleos").string()),cleosargs({"-u","http://" + add + ":" + lexical_cast<string>(pn) + "/"})
{
	wallet.create(walletname);
	if(false == exists(config_dir)) create_directory(config_dir);
}

Node::~Node()
{
	if(nodeos != nullptr && nodeos->running()) {
		nodeos->terminate();
		nodeos->wait();
	}
}

bool Node::run()
{
	return true;
}

boost::tuple<bool,string,string> Node::create_keypair()
{
	//NOTE:creating key pair doesnt need a running nodeos instance
	vector<string> arguments(cleosargs.begin(),cleosargs.end());
	arguments.push_back("create");
	arguments.push_back("key");
	io_service ios;
	future<string> buf;
	std::error_code ec;
#ifndef NDEBUG
	cout<<cleoscmd<<" ";
	for(auto & arg : arguments) cout<<arg<<" ";
	cout<<endl;
#endif
	child c(cleoscmd,arguments,std_out > buf,ios,ec);
	ios.run();
	string line;
	regex private_expr("Private key:\\s*(.*)");
	regex public_expr("Public key:\\s*(.*)");
	string publickey,privatekey;
	while(buf.valid()) {
		stringstream sstr;
		sstr<<buf.get();
		while(getline(sstr,line)) {
			match_results<string::const_iterator> what;
			if(regex_match(line,what,private_expr))
				privatekey = string(what[1].begin(),what[1].end());
			if(regex_match(line,what,public_expr))
				publickey = string(what[1].begin(),what[1].end());
		}
	}
	if(EXIT_SUCCESS != ec.value()) return boost::make_tuple(false,"","");
	//import the privatekey to managable wallet
	wallet.import(privatekey);
	return boost::make_tuple(true,publickey,privatekey);
}

bool Node::create_account(string name,string publickey,string creater)
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos node is not running!");
	trim(creater);
	if(creater.empty()) creater = walletname;
	vector<string> arguments(cleosargs.begin(),cleosargs.end());
	arguments.push_back("system");
	arguments.push_back("newaccount");
	arguments.push_back(creater);
	arguments.push_back("--transfer");
	arguments.push_back(name);
	arguments.push_back(publickey);
	arguments.push_back("--stake-net");
	arguments.push_back("100000.0000 SYS");
	arguments.push_back("--stake-cpu");
	arguments.push_back("100000.0000 SYS");
	arguments.push_back("--buy-ram-kbytes");
	arguments.push_back("8");
	io_service ios;
	future<string> buf;
	std::error_code ec;
#ifndef NDEBUG
	cout<<cleoscmd<<" ";
	for(auto & arg : arguments) cout<<arg<<" ";
	cout<<endl;
#endif
	child c(cleoscmd,arguments,std_out > buf,ios,ec);
	ios.run();
	return (EXIT_SUCCESS == ec.value())?true:false;
}

bool Node::set_contract(string name,string contract_path)
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos is not running!");
	trim(name);
	trim(contract_path);
	if(name.empty() || contract_path.empty()) return false;
	vector<string> arguments(cleosargs.begin(),cleosargs.end());
	arguments.push_back("set");
	arguments.push_back("contract");
	arguments.push_back(name);
	arguments.push_back(contract_path);
	arguments.push_back("-p");
	arguments.push_back(name);
	arguments.push_back("-x");
	arguments.push_back("60");
	io_service ios;
	future<string> buf;
	std::error_code ec;
#ifndef NDEBUG
	cout<<cleoscmd<<" ";
	for(auto & arg : arguments) cout<<arg<<" ";
	cout<<endl;
#endif
	child c(cleoscmd, arguments, std_out > buf,ios,ec);
	ios.run();
	return (EXIT_SUCCESS == ec.value())?true:false;
}

bool Node::call_contract(string name,string action,string args,string authorizer)
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos is not running!");
	//if the account who give right to call is not specified, we just use the contract owner's authority
	trim(authorizer);
	if(authorizer.empty()) authorizer = name;
	vector<string> arguments(cleosargs.begin(),cleosargs.end());
	arguments.push_back("push");
	arguments.push_back("action");
	arguments.push_back(name);
	arguments.push_back(action);
	arguments.push_back(args);
	arguments.push_back("-p");
	arguments.push_back(authorizer);
	arguments.push_back("-x");
	arguments.push_back("60");
	io_service ios;
	future<string> buf;
	std::error_code ec;
#ifndef NDEBUG
	cout<<cleoscmd<<" ";
	for(auto & arg : arguments) cout<<arg<<" ";
	cout<<endl;
#endif
	child c(cleoscmd, arguments,std_out > buf,ios,ec);
	ios.run();
	return (EXIT_SUCCESS == ec.value())?true:false;
}

bool Node::transfer(string from,string to,string quantity,string memo)
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos is not running!");
	vector<string> arguments(cleosargs.begin(),cleosargs.end());
	arguments.push_back("transfer");
	arguments.push_back(from);
	arguments.push_back(to);
	arguments.push_back(quantity);
	arguments.push_back(memo);
	io_service ios;
	future<string> buf;
	std::error_code ec;
#ifndef NDEBUG
	cout<<cleoscmd<<" ";
	for(auto & arg: arguments) cout<<arg<<" ";
	cout<<endl;
#endif
	child c(cleoscmd, arguments,std_out > buf,ios,ec);
	ios.run();
	return (EXIT_SUCCESS == ec.value())?true:false;
}

bool Node::register_producer(string accountname,string publickey)
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos is not running!");
	vector<string> arguments(cleosargs.begin(),cleosargs.end());
	arguments.push_back("system");
	arguments.push_back("regproducer");
	arguments.push_back(accountname);
	arguments.push_back(publickey);
	arguments.push_back("https://" + accountname + ".com/" + publickey);
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

boost::tuple<bool,vector<boost::tuple<string,string,string,float> > > Node::list_producers()
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos is not running! ");
	vector<string> arguments(cleosargs.begin(),cleosargs.end());
	arguments.push_back("system");
	arguments.push_back("listproducers");
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
	vector<boost::tuple<string,string,string,float> > producers;
	string line;
	regex producer("^([^ \t]*)\\s*([^ \t]*)\\s*([^ \t]*)\\s*([^ \t]*)$");
	while(buf.valid()) {
		stringstream sstr;
		sstr<<buf.get();
		while(getline(sstr,line)) {
			match_results<string::const_iterator> what;
			if(regex_match(line,what,producer))
				producers.push_back(
					boost::make_tuple(
						string(what[1].begin(),what[1].end()),
						string(what[2].begin(),what[2].end()),
						string(what[3].begin(),what[3].end()),
						lexical_cast<float>(string(what[4].begin(),what[4].end()))
					)
				);
		}
	}
	if(EXIT_SUCCESS != ec.value()) return boost::make_tuple(false,producers);
	return boost::make_tuple(true,producers);
}

bool Node::vote_producers(vector<string> producers,string voter)
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos is not running!");
	trim(voter);
	if(voter.empty()) voter = walletname;
	vector<string> arguments(cleosargs.begin(),cleosargs.end());
	arguments.push_back("system");
	arguments.push_back("voteproducer");
	arguments.push_back("prods");
	arguments.push_back(voter);
	io_service ios;
	future<string> buf;
	std::error_code ec;
	for(auto & producer : producers)
		arguments.push_back(producer);
#ifndef NDEBUG
	cout<<cleoscmd<<" ";
	for(auto & arg: arguments) cout<<arg<<" ";
	cout<<endl;
#endif
	child c(cleoscmd,arguments,std_out > buf,ios,ec);
	ios.run();
	return (EXIT_SUCCESS == ec.value())?true:false;
}

bool Node::proxy_vote(string producer,string proxy)
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos is not running!");
	vector<string> arguments(cleosargs.begin(),cleosargs.end());
	arguments.push_back("system");
	arguments.push_back("voteproducer");
	arguments.push_back("proxy");
	arguments.push_back(producer);
	arguments.push_back(proxy);
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

void Node::wait()
{
	if(nodeos == nullptr || false == nodeos->running()) throw logic_error("the nodeos is not running!");
	nodeos->wait();
}

void Node::print_message()
{
	std::ofstream log(walletname + "_log.txt");
	ios.run();
	while(buf.valid()) {
		log << buf.get();
	}
}

