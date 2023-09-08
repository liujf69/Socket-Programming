// gcc fork.c -o fork
// ./fork
#include <stdio.h>
#include <unistd.h>

int gval = 10;
int main(int argc, char *argv[]){
    __pid_t pid;
    int lval = 20;
    gval++, lval += 5;

    pid = fork();
    if(pid == 0){ // 对于子进程，fork返回0，因此执行以下内容
        gval += 2, lval += 2;
    }
    else{ // 对于父进程，执行以下内容
        gval -= 2, lval -= 2;
    }

    if(pid == 0){
        // 对于子进程，复制父进程的进程副本，则最初 gval = 11, lval = 25;
        // 执行 += 2 后，gval = 13, lval = 27;
        printf("Child Proc: [%d, %d] \n", gval, lval);
    }
    else{
        // 对于父进程，执行 -= 2后，gval = 9, lval = 23;
        printf("Parent Proc: [%d %d] \n", gval, lval);
    }
    return 0;
}