// gcc echo_client.c -o echo_client
// ./echo_client 127.0.0.1 9190

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]){
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;
    FILE* readfp;
    FILE* writefp;
    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("connect() error!");
    }
    else{
        puts("Connected.......");
    }

    readfp = fdopen(sock, "r"); // 读模式，socket文件描述符转换为 FILE*
    writefp = fdopen(sock, "w"); // 写模式，socket文件描述符转换为 FILE*
    while(1){
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")){
            break;
        }

        fputs(message, writefp); // 调用标准I/O进行读写
        fflush(writefp);
        fgets(message, BUF_SIZE, readfp);
        printf("Message from server: %s", message);
    }
    fclose(writefp);
    fclose(readfp);
    return 0;
}