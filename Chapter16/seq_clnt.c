// gcc seq_clnt.c -o seq_clnt
// ./seq_clnt 127.0.0.1 9190

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]){
    int sock;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr;

    FILE* readfp;
    FILE* writefp;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    readfp = fdopen(sock, "r"); // 基于同一个socket文件描述符的 FILE*
    writefp = fdopen(sock, "w");

    while(1){
        if(fgets(buf, sizeof(buf), readfp) == NULL){ // 客户端收到 EOF 终止循环
            break;
        }
        fputs(buf, stdout);
        fflush(stdout);
    }

    // 由于服务器端关闭了 socket，因此该消息无法发送到服务器端
    fputs("FROM CLIENT: Thank you! \n", writefp); 
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);
    return 0;
}