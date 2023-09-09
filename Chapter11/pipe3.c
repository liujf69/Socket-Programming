// gcc pipe3.c -o pipe3
// ./pipe3

#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[]){
    int fds1[2], fds2[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE];
    __pid_t pid;

    pipe(fds1), pipe(fds2); // 创建管道
    pid = fork();
    if(pid == 0){ // 子进程执行区域
        write(fds1[1], str1, sizeof(str1)); // 通过管道1写数据
        read(fds2[0], buf, BUF_SIZE); // 通过管道2读数据
        printf("Child proc output: %s \n", buf);
    }
    else{ // 父进程执行区域
        read(fds1[0], buf, BUF_SIZE); // 通过管道1读数据
        printf("Parent proc output: %s \n", buf);
        write(fds2[1], str2, sizeof(str2)); // 通过管道2写数据
        sleep(3);
    }
    return 0;
}