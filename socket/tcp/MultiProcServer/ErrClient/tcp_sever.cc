#include"tcp_server.hpp"

int main()
{
  TcpServer* tsvr = new TcpServer(8081);
  tsvr->InitTcpServer();
  tsvr->Loop();
  return 0;
}
