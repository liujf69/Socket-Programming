// gcc desto.c -o desto
// ./desto 

#include <stdio.h>
#include <fcntl.h>

int main(void){
    FILE* fp;
    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC); // 打开data.dat文件，返回其文件描述符
    if(fd == -1){
        fputs("file open error", stdout);
        return -1;
    }
    fp = fdopen(fd, "w"); // 文件描述符转换为 FILE*
    fputs("Network C programming \n", fp); // 通过标准 I/O 函数向 FILE* 指向的文件写数据
    fclose(fp); // 通过标准 I/O 函数关闭文件
    return 0;
}