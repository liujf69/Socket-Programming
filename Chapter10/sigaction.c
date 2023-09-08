// gcc sigaction.c -o sigaction
// ./sigaction

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){
    if(sig == SIGALRM){
        puts("Time out!");
    }
    alarm(2);
}

int main(int argc, char* argv[]){
    int i;
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask); // 调用sigemptyset()将sa_mask的所有位初始化为0
    act.sa_flags = 0; // sa_flags也初始化为0
    sigaction(SIGALRM, &act, 0);
    alarm(2);
    for(int i = 0; i < 3; i++){
        puts("wait...");
        sleep(100);
    }
    return 0;
}
