#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <unistd.h>

class Handler
{
    public:
        Handler()
        {}
        ~Handler()
        {}
        void operator()(int sock, std::string client_ip, int client_port)
        {
            //only for test
            std::unordered_map<std::string, std::string> dict;
            dict.insert(std::make_pair("dragon", "龙"));
            dict.insert(std::make_pair("blog", "博客"));
            dict.insert(std::make_pair("socket", "套接字"));

            char buffer[1024];
            std::string value;
            while(true){
                ssize_t size = read(sock, buffer, sizeof(buffer)-1);
                if(size > 0){ //读取成功
                    buffer[size] = '\0';
                    std::cout<<client_ip<<":"<<client_port<<"# "<<buffer<<std::endl;

                    std::string key = buffer;
                    auto it = dict.find(key);
                    if(it != dict.end()){
                        value = it->second;
                    }
                    else{
                        value = key;
                    }

                    write(sock, value.c_str(), value.size());
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
};

class Task
{
    public:
        Task()
        {}
        Task(int sock, std::string client_ip, int client_port)
            :_sock(sock)
            ,_client_ip(client_ip)
            ,_client_port(client_port)
        {}
        ~Task()
        {}
        void Run()
        {
            _handler(_sock, _client_ip, _client_port); //调用仿函数
        }
    private:
        int _sock; //套接字
        std::string _client_ip; //IP地址
        int _client_port; //端口号
        Handler _handler; //处理方法
};
