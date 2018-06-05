#include <cstdlib>
#include <iostream>
#include <boost/filesystem.hpp>
#include "GenesisNode.h"
#include "ProducerNode.h"

using namespace std;
using namespace boost::filesystem;

int main()
{
	//clean wallet and config directories
	remove_all("/root/eosio-wallet");
	create_directory("/root/eosio-wallet");
	remove_all("/root/.nodes");
	//1)create nodes
	GenesisNode genesis("genesis",{},"localhost",8888);
	genesis.run();
	boost::tuple<bool,string,string> retval1 = genesis.create_keypair(); assert(get<0>(retval1));
	boost::tuple<bool,string,string> retval2 = genesis.create_keypair(); assert(get<0>(retval2));
	boost::tuple<bool,string,string> retval3 = genesis.create_keypair(); assert(get<0>(retval3));
	boost::tuple<bool,string,string> retval4 = genesis.create_keypair(); assert(get<0>(retval4));
	boost::tuple<string,string> keypair1 = boost::make_tuple(get<1>(retval1),get<2>(retval1));
	boost::tuple<string,string> keypair2 = boost::make_tuple(get<1>(retval2),get<2>(retval2));
	boost::tuple<string,string> keypair3 = boost::make_tuple(get<1>(retval3),get<2>(retval3));
	boost::tuple<string,string> keypair4 = boost::make_tuple(get<1>(retval4),get<2>(retval4));
	//create producer nodes what link to genesis node
	ProducerNode producer1("prod1",keypair1,{boost::make_tuple("localhost",8888)},"localhost",9011,8011); producer1.run();
	ProducerNode producer2("prod2",keypair2,{boost::make_tuple("localhost",8888)},"localhost",9012,8012); producer2.run();
	ProducerNode producer3("prod3",keypair3,{boost::make_tuple("localhost",8888)},"localhost",9013,8013); producer3.run();
	ProducerNode producer4("prod4",keypair4,{boost::make_tuple("localhost",8888)},"localhost",9014,8014); producer4.run();
	//2)voting
	producer1.vote_producers({"prod2"},"prod1");
	producer2.vote_producers({"prod1"},"prod2");
	producer3.vote_producers({"prod1"},"prod3");
	producer4.vote_producers({"prod1"},"prod4");
	//3)wait for running nodes
	genesis.wait();
	producer1.wait();
	producer2.wait();
	producer3.wait();
	producer4.wait();

	return EXIT_SUCCESS;
}

