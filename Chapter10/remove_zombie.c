// gcc remove_zombie.c -o remove_zombie
// ./remove_zombie

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG); // 等待任意子线程结束
    if(WIFEXITED(status)){ // 判断子线程是否正常终止
        printf("Remove proc id: %d \n", id);
        printf("Child send: %d \n", WEXITSTATUS(status)); // 打印子线程的返回值
    }
}

int main(int argc, char *argv[]){
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc; // 设置信号处理函数
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0); // 调用sigaction()，当遇到子线程结束的信号时，调用信号处理函数

    pid = fork();
    if(pid == 0){ // 子线程执行区域
        puts("Hi! I'm child process");
        sleep(10);
        return 12;
    }
    else{
        printf("Child proc id: %d \n", pid);
        pid = fork();
        if(pid == 0){ // 另一个子线程执行区域
            puts("Hi! I'm child process");
            sleep(10);
            return 24;
        }
        else{
            int i;
            printf("Child proc id: %d \n", pid);
            for(int i = 0; i < 5; i++){
                puts("wait...");
                sleep(5);
            }
        }
    }
    return 0;
}