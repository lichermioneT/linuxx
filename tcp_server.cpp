// tcp_server.cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);   // 需要测试的端口
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 2;
    }

    listen(sock, 5);
    std::cout << "TCP server listening on port 9000..." << std::endl;

    int client = accept(sock, nullptr, nullptr);
    if (client >= 0) 
    {
        char buffer[1024] = {0};
        read(client, buffer, sizeof(buffer));
        std::cout << "received: " << buffer << std::endl;

        std::string reply = "port 9000 OK";
        write(client, reply.c_str(), reply.size());
    }

    close(sock);
    return 0;
}
