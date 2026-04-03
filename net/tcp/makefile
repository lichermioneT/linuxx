# 编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# 目标可执行文件
TARGETS = client server

# 默认目标
all: $(TARGETS)

# 编译 client
client: client.cc client.hpp
	$(CXX) $(CXXFLAGS) -o $@ client.cc

# 编译 server
server: server.cc server.hpp
	$(CXX) $(CXXFLAGS) -o $@ server.cc

# 清理生成的可执行文件
clean:
	rm -f $(TARGETS)

# 声明伪目标
.PHONY: all clean
