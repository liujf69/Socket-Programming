// gcc seq_serv2.c -o seq_serv2
// ./seq_serv2 9190

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;
    FILE* readfp;
    FILE* writefp;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    char buf[BUF_SIZE] = {0,};

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    listen(serv_sock, 5);
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

    readfp = fdopen(clnt_sock, "r"); 
    writefp = fdopen(dup(clnt_sock), "w"); // 复制文件描述符后，再创建写模式的 FILE*，实现 I/O 流分离

    fputs("FROM SERVER: Hi~ client? \n", writefp);
    fputs("I Love all of the world \n", writefp);
    fputs("You are awesome! \n", writefp);
    fflush(writefp);

    shutdown(fileno(writefp), SHUT_WR); // 设置半关闭
    fclose(writefp); // 关闭 writefp，会终止 socket，同时向客户端发送 EOF
    // 此时不会导致 socket 的关闭，因为还有一个文件描述符
    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    fclose(readfp);
    return 0;
}