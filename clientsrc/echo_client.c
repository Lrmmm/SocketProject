#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 8765
#define SERVER_IP "43.139.207.11"

int main(int argc, char* argv[]) {
    int sock;
    char* message;
    struct sockaddr_in servaddr;
    int length;
    char buf[64];

    if (argc != 2) {
        fputs("Usage: ./echo_client message \n", stderr);
        exit(1);
    }

    message = argv[1];

    printf("message: %s\n", message);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, '\0', sizeof(struct sockaddr_in));

    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr);  // 将IPv4地址字符串转换为网络地址结构中的二进制形式
    servaddr.sin_port = htons(SERVER_PORT);

    connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));  // 将Socket和网络地址绑定

    write(sock, message, strlen(message));

    length = read(sock, buf, sizeof(buf) - 1);

    if (length > 0) {
        buf[length] = '\0';
        printf("receive: %s\n", buf);
    }
    else {
        perror("error!!! \n");
    }

    printf("finished. \n");


    return 0;
} 