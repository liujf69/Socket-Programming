// gcc signal.c -o signal
// ./signal

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){
    if(sig == SIGALRM){
        puts("Time out!");
    }
    alarm(2);
}

void keycontrol(int sig){
    if(sig == SIGINT){
        puts("CTRL+C pressed");
    }
}

int main(int argc, char *argv[]){
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for(i = 0; i < 3; i++){
        puts("wait...");
        sleep(100); // 不会真的睡眠 100s，因为alarm函数会产生SIGALRM信号事件，从而唤醒进程
    }
    return 0;
}