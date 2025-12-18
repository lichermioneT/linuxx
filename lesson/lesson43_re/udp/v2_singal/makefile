cc=g++

.PHONY:all
all:udpserver udpclient

udpserver: udpServer.cc
	$(cc)	-o $@ $^
udpclient: udpClient.cc
	$(cc)	-o $@ $^

.PHONY:clean
clean:
	rm udpserver udpclient -rf
