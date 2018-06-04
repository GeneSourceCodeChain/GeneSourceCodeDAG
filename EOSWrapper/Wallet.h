#ifndef WALLET_H
#define WALLET_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class Wallet {
	string address;
	unsigned short portnum;
	const string cmd;
	const vector<string> args;
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

