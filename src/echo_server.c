#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define SERVER_PORT 8765
#define BUF_SIZE 256

int main(void)
{
    // AF_INET:ipv4网络协议  SOCK_STREAM:TCP套接字，一个面向连接的套接字  0:默认协议(TCP) 
    // 返回一个文件描述符(int)
    // int i;
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        fprintf(stderr,"Create socket error, %s \n", strerror(errno));
        exit(1);
    }

    struct sockaddr_in server_addr;    // 用于描述服务端的 IPv4 地址和端口号的数据结构
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // host to network long 将主机字节流转为网络字节流(32位)
    server_addr.sin_port = htons(SERVER_PORT);          // host to network short 将主机字节流转为网络字节流(16位)

    // 将一个套接字（socket）绑定到一个本地地址
    if (-1 == bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
        fprintf(stderr, "Bind error, %s \n", strerror(errno));
        close(sock);
        exit(1);
    } 

    // 开始监听一个指定的套接字上的连接请求
    if (-1 == listen(sock, 128)) {
        fprintf(stderr, "Listen error, %s \n", strerror(errno));
        close(sock);
        exit(1);
    }

    printf("Waiting for connection ... \n");

    while (1)
    {

        struct sockaddr_in client_addr;     // 用于描述客户端的 IPv4 地址和端口号的数据结构
        int client_sock;    // 定义客户端的Socket(信箱)
        char client_ip[64];  // 用于存储客户端的ip地址
        int buf_len;
        char buf[BUF_SIZE];

        socklen_t client_addr_len = sizeof(client_addr);
        client_sock = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len); // 阻塞并等待客户端的连接请求，如果有新的客户端连接请求，将创建一个新的套接字来处理该连接，并返回该套接字的文件描述符
        printf("client ip : %s\t port : %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip ,sizeof(client_ip))
                , ntohs(client_addr.sin_port));


        buf_len = read(client_sock, buf, sizeof(buf) - 1);
        buf[buf_len] = '\0';

        printf("len: %d ,recive: %s\n", buf_len, buf);
        // 将buf转为大写
        for (int i = 0; i < buf_len; i++)
        {
            buf[i] = toupper(buf[i]);
        }
        

        buf_len = write(client_sock, buf, buf_len); // 此时服务器通过TCP向客户端发送消息时，就不需要指定IP地址和端口号了，因为TCP是面向连接的，此时连接已经建立
        close(client_sock);
        printf("write finished!\n");
    }

    return 0;
}