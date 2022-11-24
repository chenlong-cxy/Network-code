#pragma once

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#define BACKLOG 5

class TcpServer
{
    public:
        TcpServer(int port)
            :_listen_sock(-1)
            ,_port(port)
        {}
        void InitServer()
        {
            //创建套接字
            _listen_sock = socket(AF_INET, SOCK_STREAM, 0);
            if(_listen_sock < 0){
                std::cerr<<"socket error"<<std::endl;
                exit(2);
            }
            //绑定
            struct sockaddr_in local;
            memset(&local, '\0', sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = INADDR_ANY;

            if(bind(_listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0){
                std::cerr<<"bind error"<<std::endl;
                exit(3);
            }
            //监听
            if(listen(_listen_sock, BACKLOG) < 0){
                std::cerr<<"listen error"<<std::endl;
                exit(4);
            }
        }
        void Service(int sock, std::string client_ip, int client_port)
        {
            char buffer[1024];
            while(true){
                ssize_t size = read(sock, buffer, sizeof(buffer)-1);
                if(size > 0){ //读取成功
                    buffer[size] = '\0';
                    std::cout<<client_ip<<":"<<client_port<<"# "<<buffer<<std::endl;

                    write(sock, buffer, size);
                }
                else if(size == 0){ //对端关闭连接
                    std::cout<<client_ip<<":"<<client_port<<" close!"<<std::endl;
                    break;
                }
                else{ //读取失败
                    std::cerr<<sock<<" read error!"<<std::endl;
                    break;
                }
            }
            close(sock); //归还文件描述符
            std::cout<<client_ip<<":"<<client_port<<" service done!"<<std::endl;
        }
        void Start()
        {
            signal(SIGCHLD, SIG_IGN); //忽略SIGCHLD信号
            for( ; ; ){
                //获取连接
                struct sockaddr_in peer;
                memset(&peer, '\0', sizeof(peer));
                socklen_t len = sizeof(peer);
                int sock = accept(_listen_sock, (struct sockaddr*)&peer, &len);
                if(sock < 0){
                    std::cerr<<"accept error, continue next"<<std::endl;
                    continue;
                }
                std::string client_ip = inet_ntoa(peer.sin_addr);
                int client_port = ntohs(peer.sin_port);
                std::cout<<"get a new link->"<<sock<<" ["<<client_ip<<"]:"<<client_port<<std::endl;

                pid_t id = fork();
                if(id == 0){ //child
                    //处理请求
                    Service(sock, client_ip, client_port);
                    exit(0); //子进程提供完服务退出
                }
            }
        }
        ~TcpServer()
        {
            if(_listen_sock >= 0){
                close(_listen_sock);
            }
        }
    private:
        int _listen_sock; //监听套接字
        int _port; //端口号
};
