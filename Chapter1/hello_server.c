// gcc hello_server.c -o hello_server
// ./hello_server 9190
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]){
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello World!";
    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0); // 调用 socket 函数创建socket
    if(serv_sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 调用 bind 函数分配 ip 地址和端口号
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind() error");
    }

    // 调用 listen 函数将 socket 转为可接收连接状态
    if(listen(serv_sock, 5) == -1){
        error_handling("listen() error");
    }

    clnt_addr_size = sizeof(clnt_addr);
    // 调用 accept 函数受理连接请求
    // 没有连接请求的情况下，不会有返回情况，直到有连接请求为止
    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);

    if(clnt_sock == -1){
        error_handling("accept() error");
    }
    // 调用 write 函数传输数据
    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
    
}