// gcc pipe1.c -o pipe
// ./pipe

#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[]){
    int fds[2];
    char str[] = "Who are you?";
    char buf[BUF_SIZE];
    __pid_t pid;

    pipe(fds); // 创建管道
    pid = fork();
    if(pid == 0){ // 子进程执行区域
        write(fds[1], str, sizeof(str)); // 向管道入口写数据
    }
    else{ // 父进程执行区域
        read(fds[0], buf, BUF_SIZE); // 向管道出口读数据
        puts(buf);
    }
    return 0;
}