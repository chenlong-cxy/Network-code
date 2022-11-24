CC=g++
Flags=-std=c++11 -lpthread
Src=server.cc
Target=server

$(Target):$(Src)
	$(CC) -o $@ $^ $(Flags)
.PHONY:clean
clean:
	rm -f $(Target)

