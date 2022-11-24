#pragma once 

#include<iostream>
#include<string>
#include<cstring>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<unistd.h>

class TcpClient{
  private:
    std::string svr_ip;
    int svr_port;
    int sock;
  public:
    TcpClient(std::string _ip,int _port):svr_ip(_ip),svr_port(_port),sock(-1)
    {}

    void InitTcpClient()
    {
       sock = socket(AF_INET,SOCK_STREAM,0);
      if(sock<0){
        std::cerr<<"socket error "<<std::endl;
        exit(2);

      }

      std::cout<<"socket success,sock is "<<sock<<std::endl;
    }

    void Start()
    {
        std::cout<<svr_ip<<":"<<svr_port<<std::endl;
       struct sockaddr_in peer;
       memset(&peer, '\0', sizeof(peer));
       peer.sin_family = AF_INET;
       peer.sin_port = htons(svr_port);
       peer.sin_addr.s_addr = inet_addr(svr_ip.c_str());
       
      //struct sockaddr_in peer;
      //memset(&peer,0,sizeof(peer));
      //peer.sin_family = AF_INET;
      //peer.sin_port = htons(svr_port);//主机字节序转换为网络字节序
      //peer.sin_addr.s_addr = inet_addr(svr_ip.c_str()); // c_str可以把C++字符串转成c语言风格字符
       std::cout<<sock<<std::endl;
      if(connect(sock,(struct sockaddr*)&peer,sizeof(peer))==0){
        //success
        std::cout<<" connect success ..."<<std::endl;
        Request(sock);

      }else{

        //failed
        std::cout<<" connect failed ..."<<std::endl;
        std::cout<<strerror(errno)<<std::endl;
       std::cout<<sock<<std::endl;
        
      }

    }
    void Request(int sock)
    {
      std::string message;
      char buffer[1024];
      while(true){
        std:: cout<<"Please Input Message# ";
        getline(std::cin,message);
        write(sock,message.c_str(),message.size());

        ssize_t s = read(sock,buffer,sizeof(buffer)-1);
        if(s>0)buffer[0]=0;

        std::cout<<"server echo# "<<buffer<<std::endl;
      }

    }
    ~TcpClient()
    {
      if(sock>=0){
        close(sock);
      }
    }
};
