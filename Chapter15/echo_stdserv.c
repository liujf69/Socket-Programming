// gcc echo_stdserv.c -o echo_stdserv
// ./echo_stdserv 9190

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
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;
    FILE* readfp;
    FILE* writefp;
    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error");
    }
    if(listen(serv_sock, 5) == -1){
        error_handling("listen() error");
    }
    clnt_adr_sz = sizeof(clnt_adr);

    for(i = 0; i < 5; i++){
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1){
            error_handling("accept() error");
        }
        else{
            printf("Connected client %d \n", i+1);
        }

        readfp = fdopen(clnt_sock, "r"); // 读模式，socket文件描述符转换为 FILE*
        writefp = fdopen(clnt_sock, "w"); // 写模式，socket文件描述符转换为 FILE*
        while(!feof(readfp)){
            fgets(message, BUF_SIZE, readfp); // 调用标准I/O进行读写
            fputs(message, writefp);
            fflush(writefp); // 刷新缓冲区
        }
        close(serv_sock);
        return 0;
    }
}