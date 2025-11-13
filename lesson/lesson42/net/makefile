cc=g++

.PHONY:all
all:udpServer updClient

udpServer: udpServer.cc
	$(cc)	-o $@ $^
updClient: updClient.cc
	$(cc)	-o $@ $^

.PHONY:clean
clean:
	rm udpServer updClient -rf
