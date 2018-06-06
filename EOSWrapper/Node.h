#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <system_error>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/process.hpp>
#include <boost/asio/io_service.hpp>
#include "Wallet.h"

using namespace std;
using namespace boost;
using namespace boost::process;
using namespace boost::filesystem;
using namespace boost::asio;

class Node {
protected:
	static const path config_dir;
	boost::shared_ptr<child> nodeos;
	boost::shared_ptr<boost::thread> print_msg_handle;
	const string nodeoscmd;
	const string cleoscmd;
	const vector<string> nodeosargs;
	const vector<string> cleosargs;
	io_service ios;
	future<string> buf;
	std::error_code ec;
	//genesis json
	static string initial_key;
	static string initial_timestamp;
	//serializable members
	Wallet wallet;
	string walletname;
public:
	Node();
	Node(string address,unsigned short portnum,string name);
	virtual ~Node();
	virtual bool run() = 0; //force derived class implement this function
	boost::tuple<bool,string,string> create_keypair();
	bool create_account(string accountname,string publickey,string creater = "");
	bool set_contract(string accountname,string contract_path);
	bool call_contract(string contract,string action,string arguments,string authorizer = "");
	bool transfer(string from,string to,string quantity,string memo);
	bool register_producer(string accountname, string publickey);
	boost::tuple<bool,vector<boost::tuple<string,string,string,float> > > list_producers();
	bool vote_producers(vector<string> producers,string voter = "");
	bool register_proxy(string accountname);
	bool proxy_vote(string producer,string proxy);
	void wait();
	void print_message();
};

#endif

