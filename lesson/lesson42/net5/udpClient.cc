#include "udpClient.hpp"
#include <memory>
#include <cstdlib>

using namespace Client;

static void Usage(const std::string& proc)
{
    std::cout << "Usage:\n\t" << proc << " server_ip server_port \n\n";
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    std::string serverip = argv[1];                       
    uint16_t serverport = static_cast<uint16_t>(atoi(argv[2]));

    auto ucli = std::make_unique<udpClient>(serverip, serverport);

    ucli->initClient();
    ucli->run();

    return 0;
}