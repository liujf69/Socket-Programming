// gcc thread1.c -o thread1 -lpthread
// ./thread1

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_main(void* arg){ // 线程入口函数
    int i;
    int cnt = *((int*)arg);
    for(i = 0; i < cnt; i++){
        sleep(1);
        puts("running thread");
    }
    return NULL;
}

int main(int argc, char* argv[]){
    pthread_t t_id;
    int thread_param = 5;
    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0){ // 创建线程
        puts("pthread_create() error");
        return -1;
    }
    sleep(10);
    puts("end of main");
    return 0;
}