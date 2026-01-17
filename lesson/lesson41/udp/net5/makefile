cc=g++

.PHONY:all
all:udpserver udpclient

udpserver: udpserver.cc
	$(cc)	-o $@ $^
udpclient: udpclient.cc
	$(cc)	-o $@ $^ -lpthread

.PHONY:clean
clean:
	rm udpserver udpclient -rf
