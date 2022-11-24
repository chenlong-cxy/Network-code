.PHONY:all

all:server client
	
server:server.cc
	g++ -o $@ $^ -std=c++11 -pthread
client:client.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f server client
