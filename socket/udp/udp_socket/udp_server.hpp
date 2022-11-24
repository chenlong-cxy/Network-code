#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT 8081

//INADDR_ANY
class UdpServer
{
    public:
        //UdpServer(std::string ip, int port = DEFAULT)
        //    :_port(port)
        //    ,_sockfd(-1)
        //    ,_ip(ip)
        //{};
        UdpServer(int port = DEFAULT)
            :_port(port)
            ,_sockfd(-1)
        {};
        bool InitServer()
        {
            //创建套接字
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if(_sockfd < 0){ //创建套接字失败
                std::cerr<<"socket error"<<std::endl;
                return false;
            }

            std::cout<<"socket create success, sockfd: "<<_sockfd<<std::endl;
            
            //填充网络通信相关信息
            struct sockaddr_in local;
            memset(&local, '\0', sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port);
            //local.sin_addr.s_addr = inet_addr(_ip.c_str());
            local.sin_addr.s_addr = INADDR_ANY;

            //绑定
            if(bind(_sockfd, (struct sockaddr*)&local, sizeof(sockaddr)) < 0){ //绑定失败
                std::cerr<<"bind error"<<std::endl;
                return false;
            }

            std::cout<<"bind success"<<std::endl;

            return true;
        }
        void Start()
        {
#define SIZE 128
            char buffer[SIZE];
            for( ; ; ){
                struct sockaddr_in peer;
                socklen_t len = sizeof(peer);
                ssize_t size = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &len);
                if(size > 0){
                    buffer[size] = '\0';
                    int port = ntohs(peer.sin_port);
                    std::string ip = inet_ntoa(peer.sin_addr);
                    std::cout<<ip<<":"<<port<<"# "<<buffer<<std::endl;
                }
                else{
                    std::cerr<<"recvfrom error"<<std::endl;
                }
            }
        }
        
        ~UdpServer()
        {};
    private:
        int _port; //端口号
        int _sockfd; //文件描述符
        //std::string _ip; //IP地址
};

