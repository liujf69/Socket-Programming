// gcc waitpid.c -o waitpid
// ./waitpid
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int status;
    __pid_t pid = fork();
    if(pid == 0){
        sleep(15);
        return 24;
    }
    else{
        // 没有终止的子进程时，返回0，则一直循环调用waitpid()
        // 直到有终止的子进程来跳出循环
        while(!waitpid(-1, &status, WNOHANG)){
            sleep(3);
            puts("sleep 3sec.");
        }
        if(WIFEXITED(status)){
            printf("Child send %d \n", WEXITSTATUS(status));
        }
        return 0;
    }
}