// gcc select.c -o select
// ./select
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]){
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads); // 初始化fd_set变量
    FD_SET(0, &reads); // 将文件描述符 0 对应的位设置为1，表示监视标准输入（文件描述符0对应标准输入stdin）
    
    while(1){
        temps = reads; // 记录初始值，新循环时重新初始化为初始值
        timeout.tv_sec = 5; // 超时时间设置为 5s
        timeout.tv_usec = 0;
        result = select(1, &temps, 0, 0, &timeout); // 5s内监视是否有标准输入时间发生
        if(result == -1){
            puts("select() error!");
            break;
        }
        else if(result == 0){ // 返回值为0表示超时
            puts("Time-out!");
        }
        else{
            if(FD_ISSET(0, &temps)){ // 验证是否是标准输入发生了变化，打印标准输入的内容
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s", buf);
            }
        }
    }
    return 0;
}
