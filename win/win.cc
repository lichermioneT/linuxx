#include <iostream>
#include <cstdint>

#include <string>
#include <cstring>
#include <WinSock2.h>
#include <WS2tcpip.h>   // InetPton

#pragma comment(lib, "ws2_32.lib")

using namespace std;

constexpr uint16_t SERVER_PORT = 8080;
const string SERVER_IP = "8.140.211.98";
constexpr int BUF_SIZE = 1024;

int main()
{
    // 1. 初始化 Winsock
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        cerr << "WSAStartup failed, error = " 
             << WSAGetLastError() << endl;
        return 1;
    }
    cout << "WSAStartup success" << endl;

    // 2. 创建 UDP socket
    SOCKET csock = socket(AF_INET, SOCK_DGRAM, 0);
    if (csock == INVALID_SOCKET)
    {
        cerr << "socket failed, error = " 
             << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    cout << "socket success" << endl;

    // 3. 填充服务器地址
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port   = htons(SERVER_PORT);

    if (InetPton(AF_INET, SERVER_IP.c_str(), &server.sin_addr) != 1)
    {
        cerr << "InetPton failed" << endl;
        closesocket(csock);
        WSACleanup();
        return 1;
    }

    // 4. 通信循环
    char buffer[BUF_SIZE];
    string line;

    while (true)
    {
        cout << "Please Enter # ";
        getline(cin, line);

        if (line == "quit")
        {
            cout << "client exit" << endl;
            break;
        }

        int n = sendto(
            csock,
            line.c_str(),
            static_cast<int>(line.size()),
            0,
            reinterpret_cast<sockaddr*>(&server),
            sizeof(server)
        );

        if (n == SOCKET_ERROR)
        {
            cerr << "sendto failed, error = " 
                 << WSAGetLastError() << endl;
            break;
        }

        // 接收服务器回应
        sockaddr_in peer{};
        int peerlen = sizeof(peer);
        n = recvfrom(
            csock,
            buffer,
            BUF_SIZE - 1,
            0,
            reinterpret_cast<sockaddr*>(&peer),
            &peerlen
        );

        if (n == SOCKET_ERROR)
        {
            cerr << "recvfrom failed, error = " 
                 << WSAGetLastError() << endl;
            break;
        }

        buffer[n] = '\0';
        cout << "server reply # " << buffer << endl;
    }

    // 5. 清理资源
    closesocket(csock);
    WSACleanup();
    return 0;
}
