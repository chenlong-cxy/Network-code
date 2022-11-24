CC=g++

.PHONY:all
all:udp_server udp_client

udp_server:udp_server.cc
	$(CC) -o $@ $^ -std=c++11
udp_client:udp_client.cc
	$(CC) -o $@ $^ -std=c++11 -static
	
.PHONY:clean
clean:
	rm -f udp_server udp_client

