#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>


#pragma comment(lib, "ws2_32.lib")

using namespace std;
uint16_t serverport = 8080;
string   serverip = "8.140.211.98";

int main()
{
    WSAData wsd;
    // 启动 Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        cout << "WSAStartup Error = " << WSAGetLastError() << endl;
        return 0;
    }
    else
    {
        cout << "WSAStartup Success" << endl;
    }

    SOCKET csock = socket(AF_INET, SOCK_DGRAM, 0); // UDP
    if (csock == SOCKET_ERROR)
    {
        cout << "socket error" << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "socket success" << endl;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(serverip.c_str());

#define NUM 1024
    char inbuffer[NUM];
    string line;
    while (true)
    {
        cout << "Please Enter #";

        getline(cin, line);
        int n = sendto(csock, line.c_str(), line.size(), 0, (struct sockaddr*)&server, sizeof(server));
        if (n < 0)
        {
            cerr << "sendto error" << endl;
            break;
        }
 //收数据

        inbuffer[0] = 0;
        struct sockaddr_in peer;
        int peerlen = sizeof(peer);
        n = recvfrom(csock, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr*)&peer, &peerlen);
        if (n > 0)
        {
            inbuffer[n] = 0;
            cout << "server收到的消息是# " << inbuffer << endl;
        }
        else
        {
            break;
        }
    }


    // 关闭 socket
    closesocket(csock);
    WSACleanup();
    return 0;
}
