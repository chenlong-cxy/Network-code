#include"tcp_client.hpp"


void Usage(std::string proc)
{
  std::cout<<"Usage: "<<proc<<" server_ip"<<" server_port"<<std::endl;
}

// ./client server_ip server_port
int main(int argc,char* argv[])
{
  if(argc!=3){
    Usage(argv[0]);
    exit(1);
  }
  std::string ip = argv[1];
  int port = atoi(argv[2]);
  TcpClient tcli(ip,port);
  tcli.InitTcpClient();
  tcli.Start();
  return 0;
}
