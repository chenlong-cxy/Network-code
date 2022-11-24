#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "protocol.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 3){
        cerr<<"Usage: "<<argv[0]<<" server_ip server_port"<<endl;
        exit(1);
    }
    string server_ip = argv[1];
    int server_port = atoi(argv[2]);

    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        cerr<<"socket error!"<<endl;
        exit(2);
    }

    //连接服务器
    struct sockaddr_in peer;
    memset(&peer, 0, sizeof(peer));
    peer.sin_family = AF_INET;
    peer.sin_port = htons(server_port);
    peer.sin_addr.s_addr = inet_addr(server_ip.c_str());
    if(connect(sock, (struct sockaddr*)&peer, sizeof(peer)) < 0){
        cerr<<"connect failed!"<<endl;
        exit(3);
    }

    //发起请求
    while(true){
        //构建请求
        request_t rq;
        cout<<"请输入左操作数# ";
        cin>>rq.x;
        cout<<"请输入右操作数# ";
        cin>>rq.y;
        cout<<"请输入需要进行的操作[+-*/%]# ";
        cin>>rq.op;
        send(sock, &rq, sizeof(rq), 0);

        //接收请求响应
        response_t rp;
        recv(sock, &rp, sizeof(rp), 0);
        cout<<"status: "<<rp.code<<endl;
        cout<<rq.x<<rq.op<<rq.y<<"="<<rp.result<<endl;
    }
    return 0;
}
