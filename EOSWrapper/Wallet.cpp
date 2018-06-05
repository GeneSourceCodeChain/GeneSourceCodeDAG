#include <sstream>
#include <iostream>
#include <fstream>
#include <boost/process.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "Wallet.h"

using namespace std;
using namespace boost;
using namespace boost::algorithm;
using namespace boost::process;
using namespace boost::property_tree;
using namespace boost::serialization;
using namespace boost::archive;

Wallet::Wallet(string add,unsigned short p)
:address(add),portnum(p),cmd(search_path("cleos").string()),args({"-u","http://" + address + ":" + lexical_cast<string>(portnum)+"/","wallet"})
{
}

Wallet::~Wallet()
{
}

bool Wallet::open(string name,string psw)
{
	vector<string> arguments(args.begin(),args.end());
	arguments.push_back("open");
	arguments.push_back("-n");
	arguments.push_back(name);
	child c(cmd,arguments,ec);
	c.wait();
	if(EXIT_SUCCESS == ec.value()) {
		walletname = name;
		password = psw;
		return true;
	} else return false;
}

bool Wallet::create(string name)
{
	trim(name);
	if(name.empty()) name = "default";
	vector<string> arguments(args.begin(),args.end());
	arguments.push_back("create");
	arguments.push_back("-n");
	arguments.push_back(name);
	child c(cmd,arguments,std_out > buf,ios,ec);
	ios.run();
	//parse generated password from console
	string line;
	regex psw("^\"(.*)\"$");
	string p;
	while(buf.valid()) {
		stringstream sstr;
		sstr << buf.get();
		while(getline(sstr,line)) {
			match_results<string::const_iterator> results;
			if(regex_match(line,results,psw)) {
				p = string(results[1].begin(),results[1].end());
				break;
			}
		}
	}
	if(EXIT_SUCCESS == ec.value()) {
		walletname = name;
		password = p;
		return true;
	} else return false;
}

bool Wallet::lock()
{
	if(walletname.empty()) return false;
	vector<string> arguments(args.begin(),args.end());
	arguments.push_back("lock");
	arguments.push_back("-n");
	arguments.push_back(walletname);
	child c(cmd,arguments,ec);
	c.wait();
	return (EXIT_SUCCESS == ec.value())?true:false;
}

bool Wallet::unlock()
{
	if(walletname.empty()) return false;
	//check if the account is created by this object, is other word whether we know the password
	ipstream out;
	opstream in;
	vector<string> arguments(args.begin(),args.end());
	arguments.push_back("unlock");
	arguments.push_back("-n");
	arguments.push_back(walletname);
	child c(cmd,arguments,std_out > out, std_in < in,ec);
#if 0
	//wait until password prompt is shown
	string line;
	while(out && getline(out,line))
		if(line == "password:") {
			in << password << endl;
			break;
		}
#else
	in << password << endl;
#endif
	c.wait();
	return (EXIT_SUCCESS == ec.value())?true:false;
}

bool Wallet::import(string privatekey)
{
	if(walletname.empty()) return false;
	vector<string> arguments(args.begin(),args.end());
	arguments.push_back("import");
	arguments.push_back("-n");
	arguments.push_back(walletname);
	arguments.push_back(privatekey);
	child c(cmd,arguments,ec);
	c.wait();
	return (EXIT_SUCCESS == ec.value())?true:false;
}

bool Wallet::save(string file)
{
	std::ofstream out(file);
	if(false == out.is_open()) return false;
	text_oarchive oa(out);
	oa << walletname << password;
	return true;
}

bool Wallet::load(string file)
{
	std::ifstream in(file);
	if(false == in.is_open()) return false;
	text_iarchive ia(in);
	ia >> walletname >> password;
	return true;
}

vector<string> Wallet::list()
{
	ipstream out;
	vector<string> arguments(args.begin(),args.end());
	arguments.push_back("list");
	child c(cmd,arguments,std_out > out,ec);
	string json;
	string line;
	while(out && getline(out,line))
		json += line;
	regex key("Wallets");
	regex_replace(json,key,"\"Wallets\"");
#ifndef NDEBUG
	cout<<json<<endl;
#endif
	stringstream sstr;
	sstr<<json;
	ptree pt;
	read_json(sstr,pt);
	cout<<pt.get_child("Wallets").size()<<endl;
	regex name("(.*)\\s?\\*?$");
	vector<string> wallets;
	for(ptree::value_type & wallet : pt.get_child("Wallets")) {
		string value = wallet.second.data();
		match_results<string::const_iterator> results;
		assert(regex_match(value,results,name));
		wallets.push_back(string(results[1].begin(),results[1].end()));
	}
	c.wait();
	return wallets;
}

