cc=g++

.PHONY:all
all:udpServer udpClient

udpServer: udpServer.cc
	$(cc)	-o $@ $^
updClient: udpClient.cc
	$(cc)	-o $@ $^

.PHONY:clean
clean:
	rm udpServer udpClient -rf
