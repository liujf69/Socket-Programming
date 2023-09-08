// gcc wait.c -o wait
// ./wait
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    int status;
    __pid_t pid = fork();

    if(pid == 0){
        return 3; // 第一个子进程返回3
    }
    else{
        printf("Child PID: %d \n", pid); // 第一个子进程的 ID
        pid = fork(); // 创建第二个子进程
        if(pid == 0){
            exit(7); // 第二个子进程返回7
        }
        else{
            printf("Child PID : %d \n", pid); // 第二个子进程的 ID
            wait(&status); // 主动请求获取子进程的返回值
            if(WIFEXITED(status)){
                printf("Chile send one: %d \n", WEXITSTATUS(status));
            }
            wait(&status); // 主动请求获取子进程的返回值
            if(WIFEXITED(status)){
                printf("Child send two: %d \n", WEXITSTATUS(status));
            }
            sleep(30); // 这时候父进程选择睡眠，子进程也不会成为僵尸进程
        }
    }
    return 0;
}