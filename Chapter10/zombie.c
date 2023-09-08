// gcc zombie.c -o zombie
// ./zombie
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    __pid_t pid = fork();
    if(pid == 0){
        puts("Hi, I am a child process");
    }
    else{
        // 父进程终止时，子进程也会被同时销毁
        // 本案例通过延缓父进程的终止时间，来让子进程进入僵尸进程状态
        printf("Child Process ID: %d \n", pid);
        sleep(30);
    }

    if(pid == 0){
        puts("End child process");
    }
    else{
        puts("End parent process");
    }
    return 0;
}