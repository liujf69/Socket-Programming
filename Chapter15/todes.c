// gcc todes.c -o todes
// ./todes

#include <stdio.h>
#include <fcntl.h>

int main(void){
    FILE* fp;
    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC); // 返回文件描述符
    if(fd == -1){
        fputs("file open error", stdout);
        return -1;
    }

    printf("First file descriptor: %d \n", fd);
    fp = fdopen(fd, "w"); // 文件描述符转换为 FILE*
    fputs("TCP/IP SOCKET PROGRAMMING \n", fp);
    printf("Second file descriptor: %d \n", fileno(fp)); // FILE* 转换为文件描述符
    fclose(fp);
    return 0;
}