// gcc tcp_server_win.c -o tcp_server_win -lwsock32
// tcp_server_win 9190
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

// 打印错误信息
void ErrorHandling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;
    
    int szClntAddr;
    char message[] = "Hello World!";
    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){ // 初始化套接字库
        ErrorHandling("WSAStartup() error!");
    }

    hServSock = socket(PF_INET, SOCK_STREAM, 0); // 创捷socket
    if(hServSock == INVALID_SOCKET){
        ErrorHandling("socket() error!");
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1])); // 端口

    if(bind(hServSock, (SOCKADDR*) &servAddr, sizeof(servAddr)) == SOCKET_ERROR){ // 给 socket 分配 ip 地址和端口号
        ErrorHandling("bind() error!");
    }

    if(listen(hServSock, 5) == SOCKET_ERROR){ // 置于listen状态，使socket可接收客户端连接请求
        ErrorHandling("listen() error!");
    }

    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr); // 使socket受理客户端的连接请求
    if(hClntSock == INVALID_SOCKET){
        ErrorHandling("accept() error!");
    }

    send(hClntSock, message, sizeof(message), 0); // 向连接的客户端发送数据
    closesocket(hClntSock);
    closesocket(hServSock);
    WSACleanup(); // 注销初始化的套接字库
    return 0;
}