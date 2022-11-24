#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class TcpClient
{
    public:
        TcpClient(std::string server_ip, int server_port)
            :_sock(-1)
            ,_server_ip(server_ip)
            ,_server_port(server_port)
        {}
        void InitClient()
        {
            //创建套接字
            _sock = socket(AF_INET, SOCK_STREAM, 0);
            if(_sock < 0){
                std::cerr<<"socket error"<<std::endl;
                exit(2);
            }
        }
        void Request()
        {
            std::string msg;
            char buffer[1024];
            while(true){
                std::cout<<"Please Enter# ";
                getline(std::cin, msg);

                write(_sock, msg.c_str(), msg.size());

                ssize_t size = read(_sock, buffer, sizeof(buffer)-1);
                if(size > 0){
                    buffer[size] = '\0';
                    std::cout<<"server echo# "<<buffer<<std::endl;
                }
                else if(size == 0){
                    std::cout<<"server close!"<<std::endl;
                    break;
                }
                else{
                    std::cerr<<"read error!"<<std::endl;
                    break;
                }
            }
        }
        void Start()
        {
            struct sockaddr_in peer;
            memset(&peer, '\0', sizeof(peer));
            peer.sin_family = AF_INET;
            peer.sin_port = htons(_server_port);
            peer.sin_addr.s_addr = inet_addr(_server_ip.c_str());

            if(connect(_sock, (struct sockaddr*)&peer, sizeof(peer)) == 0){ //connect success
                std::cout<<"connect success..."<<std::endl;
                Request(); //发起请求
            }
            else{ //connect error
                std::cerr<<"connect failed..."<<std::endl;
                exit(3);
            }
        }
        ~TcpClient()
        {
            if(_sock >= 0){
                close(_sock);
            }
        }
    private:
        int _sock; //套接字
        std::string _server_ip; //服务端IP地址
        int _server_port; //服务端端口号
};
