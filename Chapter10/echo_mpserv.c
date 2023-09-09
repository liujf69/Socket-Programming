// gcc echo_mpserv.c -o echo_mpserv
// ./echo_mpserv 9190

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_childproc(int sig){
    __pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("remove proc id: %d \n", pid);
}

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;

    __pid_t pid;

    struct sigaction act; // 信号
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 防止僵尸进程
    act.sa_handler = read_childproc; //设置信号处理函数
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0); // 创建 tcp socket
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error"); 
    } 
    if(listen(serv_sock, 5) == -1){
        error_handling("listen() error");
    }

    while(1){
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock == -1){
            continue;
        }
        else{
            puts("new client connected...");
        }
        // 每当有客户端请求服务时，clnt_sock 不为 -1
        // 因此会执行 fork() 函数创建子进程，并由子进程向客户端提供服务
        pid = fork(); 
        if(pid == -1){
            close(clnt_sock);
            continue;
        }
        if(pid == 0){ // 子进程运行区域
            close(serv_sock); // 将复制过来的父进程中的服务器文件描述符销毁
            while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0){
                write(clnt_sock, buf, str_len);
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
        else{ // 父进程运行区域
            // 因为客户端的文件描述符已经复制到子进程中
            // 由子进程处理客户端的内容，因此父进程需要销毁客户端的文件描述符
            close(clnt_sock); 
        }
    }
    close(serv_sock);
    return 0;
}