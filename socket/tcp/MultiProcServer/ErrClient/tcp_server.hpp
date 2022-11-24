#pragma once 

#include<iostream>
#include<cstring>
#include<string>
#include<sys/wait.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define DFL_PORT 8081
#define BACKLOG 5

class TcpServer{
  private:
    int port;
    int listen_sock;
  public:
    TcpServer(int _port = DFL_PORT):port(_port),listen_sock(-1)
    {}
    bool InitTcpServer()
    {
      listen_sock = socket(AF_INET,SOCK_STREAM,0);
      if(listen_sock<0){
        std::cerr<<"socket error "<<std::endl;
        return false;
      }

      struct sockaddr_in local;
      memset(&local,0,sizeof(local));
      local.sin_family = AF_INET;
      local.sin_port = htons(port);
      local.sin_addr.s_addr = INADDR_ANY;

      if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local))<0){
        std::cerr<<"bind error"<<std::endl;
        return false;

      }

      if(listen(listen_sock,BACKLOG)<0){
        std::cerr<<"listen error "<<std::endl;
        return false;
      }

      std:: cout<<"bind success,sock is"<< listen_sock<<" ,listening...."<<std::endl;

      return true;
    }

    void Loop()
    {
      struct sockaddr_in peer;
      for(;;){
        socklen_t len = sizeof(peer);
       int sock =  accept(listen_sock,(struct sockaddr*)&peer,&len);
       if(sock<0){
         std::cerr<<" accept error"<<std::endl;
         continue;
       
       }
       std::string ip = inet_ntoa(peer.sin_addr);
       int port = ntohs(peer.sin_port);
       std::cout<<"get a new link:[ "<< ip <<"]"<<port<<std::endl;
      
       Service(sock,ip,port);
      }
    }

    void Service(int sock,std::string ip,int port)
    {
      char buffer[1024];
      while(true){
        ssize_t size = read(sock,buffer,sizeof(buffer)-1);
        if(size>0){
          buffer[size] = 0;
          std::cout<<ip<<":"<<port<< "# "<<buffer<<std::endl;
          write(sock,buffer,size);
        }else if(size == 0){
          std::cout<<ip<<":"<<port<<" close!"<<std::endl;
          break;

        }else{
          std::cerr<<sock<<"read error"<<std::endl;
          break;

        }
      }

      close(sock);
      std::cout<<" Service done!"<<std::endl;

    }

    ~TcpServer()
    {
      if(listen_sock>=0){
        close(listen_sock);
      }
    }
};
