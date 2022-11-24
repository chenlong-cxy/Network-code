#include"tcp_server.hpp"

void Usage(std::string proc)
{
  std::cout<<"Usge: "<<proc<<" port"<<std::endl;
}

// ./tcp_server port
int main(int argc,char* argv[])
{
  if(argc!=2){
    Usage(argv[0]);
    exit(1);
  }

  TcpServer tsvr(atoi(argv[1]));
  tsvr.InitTcpServer();
  tsvr.Loop();
  return 0;
}
