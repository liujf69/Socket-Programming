// gcc echo_selectserv.c -o echo_selectserv
// ./echo_selectserv 9190
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(char *buf){
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];
    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
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

    FD_ZERO(&reads); // 初始化fd_set变量
    FD_SET(serv_sock, &reads); // 监视 serv_sock
    fd_max = serv_sock;

    while(1){
        cpy_reads = reads; // 记录初始值
        timeout.tv_sec = 5; // 设置超时时间
        timeout.tv_usec = 5000;

        if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1){
            break;
        }
        if(fd_num = 0) continue; // 判断是否是超时
        // 真的有事件发生，执行以下代码
        for(i = 0; i < fd_max + 1; i++){
            if(FD_ISSET(i, &cpy_reads)){ // 查找发生状态变化的文件描述符
                if(i == serv_sock){ // 服务器端socket有变化，执行受理连接请求
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads); // 将客户端socket注册到fd_set变量中
                    if(fd_max < clnt_sock){
                        fd_max = clnt_sock;
                    }
                    printf("connected client: %d \n", clnt_sock);
                }
                else{ // 不是服务器端socket发生变化，表明有要接收的数据
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0){ // 接收的是 EOF，表明要断开连接
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d \n", i);
                    }
                    else{ // 接收的是真实数据
                        write(i, buf, str_len); // 将接收到的数据返回客户端，实现回声功能
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}