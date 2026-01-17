cc=g++

.PHONY:all
all:udpserver udpclient

udpserver: udpserver.cc
	$(cc)	-o $@ $^
udpclient: udpclient.cc
	$(cc)	-o $@ $^

.PHONY:clean
clean:
	rm udpserver udpclient -rf
