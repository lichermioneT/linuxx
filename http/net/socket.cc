#include "socket.hpp"

Socket::Socket(uint16_t port)
    : _sockfd(-1)
    , _port(port)
{}

Socket::~Socket()
{
    Close();
}

int Socket::CreateSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "socket failed: " << strerror(errno) << std::endl;
        exit(1);
    }
    return sock;
}

void Socket::SetReuseAddr()
{
    int opt = 1;
    if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
        exit(1);
    }
}

void Socket::Bind()
{
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(_port);

    if (bind(_sockfd, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        std::cerr << "bind failed: " << strerror(errno) << std::endl;
        exit(1);
    }
}

void Socket::Listen(int backlog)
{
    if (listen(_sockfd, backlog) < 0)
    {
        std::cerr << "listen failed: " << strerror(errno) << std::endl;
        exit(1);
    }
}

int Socket::Accept(std::string* client_ip, uint16_t* client_port)
{
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);

    int sock = accept(_sockfd, (struct sockaddr*)&peer, &len);
    if (sock < 0)
    {
        return -1;
    }

    if (client_ip)
    {
        *client_ip = inet_ntoa(peer.sin_addr);
    }

    if (client_port)
    {
        *client_port = ntohs(peer.sin_port);
    }

    return sock;
}

void Socket::Init()
{
    _sockfd = CreateSocket();
    SetReuseAddr();
    Bind();
    Listen();

    std::cout << "listen success, port: " << _port << std::endl;
}

void Socket::Start(func_t handler)
{
    while (true)
    {
        std::string client_ip;
        uint16_t client_port = 0;

        int sock = Accept(&client_ip, &client_port);
        if (sock < 0)
        {
            std::cerr << "accept failed: " << strerror(errno) << std::endl;
            continue;
        }

        std::cout << "get a new connect: "
                  << client_ip << ":" << client_port << std::endl;

        handler(sock);   // 在这里调用你的 handlerHttp
        close(sock);
    }
}

void Socket::Close()
{
    if (_sockfd >= 0)
    {
        close(_sockfd);
        _sockfd = -1;
    }
}
