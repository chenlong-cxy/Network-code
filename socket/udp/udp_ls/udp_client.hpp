#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class UdpClient
{
    public:
        UdpClient(std::string server_ip, int server_port)
            :_sockfd(-1)
            ,_server_ip(server_ip)
            ,_server_port(server_port)
        {}
        bool InitClient()
        {
            //创建套接字
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if(_sockfd < 0){
                std::cerr<<"socket create error"<<std::endl;
                return false;
            }

            return true;
        }
        void Start()
        {
            std::string msg;
            struct sockaddr_in peer;
            memset(&peer, '\0', sizeof(peer));
            peer.sin_family = AF_INET;
            peer.sin_port = htons(_server_port);
            peer.sin_addr.s_addr = inet_addr(_server_ip.c_str());

            for( ; ; ){
                std::cout<<"Please Enter# ";
                getline(std::cin, msg);
                sendto(_sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr*)&peer, sizeof(peer));

#define SIZE 1024
                char buffer[SIZE];
                struct sockaddr_in tmp;
                socklen_t len = sizeof(tmp);
                ssize_t size = recvfrom(_sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&tmp, &len);
                if(size > 0){
                    buffer[size] = '\0';
                    std::cout<<buffer<<std::endl;
                }
            }
        }
        ~UdpClient()
        {
            if(_sockfd >= 0)
                close(_sockfd);
        }
    private:
        int _sockfd;
        std::string _server_ip;
        int _server_port;
};
