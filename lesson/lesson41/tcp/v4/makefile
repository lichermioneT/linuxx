.PHONY:all
all: tcpserver tcpclient 

tcpserver : tcpServer.cc
	g++ $^ -o $@ -lpthread
tcpclient : tcpClient.cc
	g++ $^ -o $@

.PHONY:clean
clean:
	rm -rf tcpclient tcpserver
