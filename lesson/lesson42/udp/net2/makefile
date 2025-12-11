cc=g++

.PHONY:all
all:udpServer udpClient

udpServer: udpServer.cc
	$(cc)	-o $@ $^
udpClient: udpClient.cc
	$(cc)	-o $@ $^

.PHONY:clean
clean:
	rm udpServer udpClient -rf
