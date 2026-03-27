# 编译器
CXX = g++

# 可执行文件名
TARGET = a.out


# 源文件
SRCS = main.cc \
	   server.cc \
	   net/socket.cc \
	   net/connection.cc \
	   http/http_parser.cc \
	   http/http_protocol.cc \
	   logic/router.cc \
	   logic/file_handler.cc \
	   util/util.cc

# 目标文件
OBJS = $(SRCS:.cc=.o)

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

# 编译 .cc -> .o
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 运行
run: $(TARGET)
	./$(TARGET)

# 清理
clean:
	rm -f $(TARGET) $(OBJS)

# 伪目标
.PHONY: all run clean
