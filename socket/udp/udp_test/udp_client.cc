#include "udp_client.hpp"


int main(int argc, char* argv[])
{
    if(argc != 3){
        std::cerr<<"Usage: "<<argv[0]<<" server_ip server_port"<<std::endl;
        return 1;
    }
    std::string server_ip = argv[1];
    int server_port = atoi(argv[2]);
    UdpClient* clt = new UdpClient(server_ip, server_port);
    clt->InitClient();
    clt->Start();
    return 0;
}
