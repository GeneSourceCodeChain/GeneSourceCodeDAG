BOOST_PREFIX=/root/opt/boost
CXXFLAGS=-I${BOOST_PREFIX}/include -Iboost/include -std=c++14 -O2 -DCONFIGDIR="\"/root/.nodes\""
LIBS=-L${BOOST_PREFIX}/lib -lboost_filesystem -lboost_system -lboost_thread -lboost_regex -lboost_serialization -lpthread 
OBJS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: demo

demo: main.o GenesisNode.o ProducerNode.o Node.o Wallet.o
	$(CXX) $^ $(LIBS) -o ${@}

clean:
	$(RM) demo $(OBJS)

