// gcc tcp_client_win.c -o tcp_client_win -lwsock32
// tcp_client_win 127.0.0.1 9190
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
    int strLen = 0;
    int idx = 0, readLen = 0;
    if(argc != 3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){ // 初始化套接字库
        ErrorHandling("WSAStartup() error!"); 
    }

    hSocket = socket(PF_INET, SOCK_STREAM, 0); // 创捷面向连接的 TCP Socket
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

    while(readLen = recv(hSocket, &message[idx++], 1, 0)){ // 接收服务器发来的数据
        if(readLen == -1) ErrorHandling("read() error!");
        strLen += readLen;
        if(message[idx - 1] == '\0') break; // 读取到最后一个字节
    }
    
    printf("Message from server: %s \n", message); 
    printf("Function read call count: %d \n", strLen);

    closesocket(hSocket);
    WSACleanup(); // 注销初始化的Winsock库
    return 0;
}