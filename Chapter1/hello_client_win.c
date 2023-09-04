// gcc hello_client_win.c -o hello_client_win -lwsock32
// hello_client_win 127.0.0.1 9190
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
    SOCKET hSocket;
    SOCKADDR_IN servAddr;

    char message[30];
    int strLen;
    if(argc != 3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){ // 初始化套接字库
        ErrorHandling("WSAStartup() error!"); 
    }

    hSocket = socket(PF_INET, SOCK_STREAM, 0); // 创捷socket
    if(hSocket == INVALID_SOCKET){
        ErrorHandling("socket() error!");
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR){ // 向服务器端发送连接请求
        ErrorHandling("connect() error!");
    }

    strLen = recv(hSocket, message, sizeof(message) - 1, 0); // 接收服务器发来的数据
    if(strLen == -1){
        ErrorHandling("read() error!");
    }
    printf("Message from server: %s \n", message); // 注销初始化的Winsock库

    closesocket(hSocket);
    WSACleanup();
    return 0;
}