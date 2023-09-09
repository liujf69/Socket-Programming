// gcc pipe2.c -o pipe2
// ./pipe2

#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[]){
    int fds[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE];
    __pid_t pid;

    pipe(fds); // 创建管道
    pid = fork();
    if(pid == 0){ // 子进程执行区域
        write(fds[1], str1, sizeof(str1)); 
        sleep(2); // sleep的作用是防止子线程写的数据被子线程自身读取了，导致父进程一直等待
        read(fds[0], buf, BUF_SIZE);
        printf("Child proc output: %s \n", buf);
    }
    else{ // 父进程执行区域
        read(fds[0], buf, BUF_SIZE); 
        printf("Parent proc output: %s \n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}