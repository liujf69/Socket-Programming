// gcc thread4.c -D_REENTRANT -o thread4 -lpthread
// ./thread4

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100

long long num = 0;

void* thread_inc(void* arg){
    int i;
    for(i = 0; i < 50000000; i++){
        num += 1;
    }
    return 0;
}

void* thread_des(void* arg){
    int i;
    for(i = 0; i < 50000000; i++){
        num -= 1;
    }
    return 0;
}

int main(int argc, char* argv[]){
    pthread_t thread_id[NUM_THREAD];
    int i;

    printf("sizeof long long: %ld \n", sizeof(long long));
    for(i = 0; i < NUM_THREAD; i++){
        // 各创建50个线程，分别执行对全局变量 num 的加减操作
        if(i%2){
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        }
        else{
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
        }
    }

    for(i = 0; i < NUM_THREAD; i++){
        pthread_join(thread_id[i], NULL);
    }

    printf("result: %lld \n", num);
    return 0;
}