#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_PORT 8765

int main(void)
{
    // AF_INET:ipv4网络协议  SOCK_STREAM:TCP套接字，一个面向连接的套接字  0:默认协议(TCP) 
    // 返回一个文件描述符(int)
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;    // 用于描述服务端的 IPv4 地址和端口号的数据结构
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    // 将一个套接字（socket）绑定到一个本地地址
    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 开始监听一个指定的套接字上的连接请求
    listen(sock, 128);

    printf("Waiting for connection ... \n");

    while (1)
    {

        struct sockaddr_in client_addr;     // 用于描述客户端的 IPv4 地址和端口号的数据结构
        int client_sock;    // 定义客户端的Socket(信箱)
        char client_ip[64];  // 用于存储客户端的ip地址

        socklen_t client_addr_len = sizeof(client_addr);
        client_sock = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len); // 等待客户端的连接请求，如果有新的客户端连接请求，将创建一个新的套接字来处理该连接，并返回该套接字的文件描述符
        printf("client ip : %s\t port : %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip ,sizeof(client_ip))
                , ntohs(client_addr.sin_port));
    }

    return 0;
}