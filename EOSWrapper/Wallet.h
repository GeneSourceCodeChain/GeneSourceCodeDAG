#ifndef WALLET_H
#define WALLET_H

#include <vector>
#include <string>
#include <map>
#include <system_error>
#include <boost/asio/io_service.hpp>

using namespace std;
using namespace boost::asio;

class Wallet {
	string address;
	unsigned short portnum;
	const string cmd;
	const vector<string> args;
	io_service ios;
	future<string> buf;
	std::error_code ec;
	//serializable
	string walletname;
	string password;
public:
	Wallet(string address = "localhost",unsigned short portnum = 8888);
	virtual ~Wallet();
	bool open(string wallet,string psw);
	bool create(string wallet = "default");
	bool lock();
	bool unlock();
	bool import(string privatekey);
	bool save(string file);
	bool load(string file);
	vector<string> list();
};

#endif

