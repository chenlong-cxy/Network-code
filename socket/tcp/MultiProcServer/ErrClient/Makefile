CC=g++

.PHONY:all
all:tcp_client tcp_server 

tcp_client:tcp_client.cc
	$(CC) -o $@ $^ -std=c++11 

tcp_server:tcp_server.cc
	$(CC) -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f tcp_client tcp_server
