// gcc echo_epollserv.c -o echo_epollserv
// ./echo_epollserv 9190

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events; 
    struct epoll_event event; // 发生时间的文件描述符结构体
    int epfd, event_cnt;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
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

    epfd = epoll_create(EPOLL_SIZE); // 创建保存 epoll 文件描述符的空间
    ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    
    event.events = EPOLLIN; // 设置监视需要读取数据的情况
    event.data.fd = serv_sock; // 设置监视的文件描述符
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event); // 将 serv_sock 注册到 epoll 例程中

    while(1){
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1); // 等待事件的发生
        if(event_cnt == -1){
            puts("epoll_wait() error");
            break;
        }

        for(i = 0; i < event_cnt; i++){ // 遍历发生事件数
            if(ep_events[i].data.fd == serv_sock){ // 当发生时间的文件描述符等于设置的 serv_sock 时
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client: %d \n", clnt_sock);
            }
            else{
                str_len = read(ep_events[i].data.fd, buf, BUF_SIZE); // 接收数据
                if(str_len == 0){ // 接收的数据是 EOF，则关闭连接
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL); // 收到EOF，删除注册的文件描述符
                    close(ep_events[i].data.fd);
                    printf("closed client: %d \n", ep_events[i].data.fd);
                }
                else{ // 将读取的数据返回给客户端，实现回声的功能
                    write(ep_events[i].data.fd, buf, str_len); // echo
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}