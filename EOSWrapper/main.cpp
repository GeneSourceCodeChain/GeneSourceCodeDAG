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
	GenesisNode genesis(
		"genesis",
		{
			boost::make_tuple("127.0.0.1",9011),
			boost::make_tuple("127.0.0.1",9012),
			boost::make_tuple("127.0.0.1",9013),
			boost::make_tuple("127.0.0.1",9014)
		},
		"127.0.0.1",9010,8010
	);
	genesis.run();
	boost::tuple<bool,string,string> retval1 = genesis.create_keypair(); assert(get<0>(retval1));
	boost::tuple<bool,string,string> retval2 = genesis.create_keypair(); assert(get<0>(retval2));
	boost::tuple<bool,string,string> retval3 = genesis.create_keypair(); assert(get<0>(retval3));
	boost::tuple<bool,string,string> retval4 = genesis.create_keypair(); assert(get<0>(retval4));
	genesis.create_account("prod1",get<1>(retval1),"eosio");
	genesis.create_account("prod2",get<1>(retval2),"eosio");
	genesis.create_account("prod3",get<1>(retval3),"eosio");
	genesis.create_account("prod4",get<1>(retval4),"eosio");
	genesis.register_producer("prod1",get<1>(retval1));
	genesis.register_producer("prod2",get<1>(retval2));
	genesis.register_producer("prod3",get<1>(retval3));
	genesis.register_producer("prod4",get<1>(retval4));
	//create producer nodes what link to genesis node
	ProducerNode producer1(
		"prod1",
		boost::make_tuple(get<1>(retval1),get<2>(retval1)),
		{
			boost::make_tuple("127.0.0.1",9010),
			boost::make_tuple("127.0.0.1",9012),
			boost::make_tuple("127.0.0.1",9013),
			boost::make_tuple("127.0.0.1",9014)
		},
		"127.0.0.1",9011,8011
	);
	producer1.run();
	ProducerNode producer2(
		"prod2",
		boost::make_tuple(get<1>(retval2),get<2>(retval2)),
		{
			boost::make_tuple("127.0.0.1",9010),
			boost::make_tuple("127.0.0.1",9011),
			boost::make_tuple("127.0.0.1",9013),
			boost::make_tuple("127.0.0.1",9014)
		},
		"localhost",9012,8012
	);
	producer2.run();
	ProducerNode producer3(
		"prod3",
		boost::make_tuple(get<1>(retval3),get<2>(retval3)),
		{
			boost::make_tuple("127.0.0.1",9010),
			boost::make_tuple("127.0.0.1",9011),
			boost::make_tuple("127.0.0.1",9012),
			boost::make_tuple("127.0.0.1",9014)
		},
		"localhost",9013,8013
	);
	producer3.run();
	ProducerNode producer4(
		"prod4",
		boost::make_tuple(get<1>(retval4),get<2>(retval4)),
		{
			boost::make_tuple("127.0.0.1",9010),
			boost::make_tuple("127.0.0.1",9011),
			boost::make_tuple("127.0.0.1",9012),
			boost::make_tuple("127.0.0.1",9013)
		},
		"localhost",9014,8014
	);
	producer4.run();
	//2)voting
	genesis.vote_producers({"prod1"},"prod1");
	genesis.vote_producers({"prod1"},"prod2");
	genesis.vote_producers({"prod1"},"prod3");
	genesis.vote_producers({"prod1"},"prod4");
	//3)wait for running nodes
	genesis.wait();
	producer1.wait();
	producer2.wait();
	producer3.wait();
	producer4.wait();

	return EXIT_SUCCESS;
}

