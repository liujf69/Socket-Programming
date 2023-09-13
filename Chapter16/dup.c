// gcc dup.c -o dup
// ./dup

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    int cfd1, cfd2;
    char str1[] = "Hi~ \n";
    char str2[] = "It's nice day~ \n";

    cfd1 = dup(1); // 复制标准输出
    cfd2 = dup2(cfd1, 7); // 复制标准输出，指定生成的文件描述符 cfd2 为 7

    printf("fd1=%d, fd2=%d \n", cfd1, cfd2);
    write(cfd1, str1, sizeof(str1)); // 写到标准输出
    write(cfd2, str2, sizeof(str2));

    close(cfd1); // 关闭复制的文件描述符
    close(cfd2);
    write(1, str1, sizeof(str1)); // 还能输出，因为文件描述符1还没关闭
    close(1);
    write(1, str2, sizeof(str2)); // 不能输出
    return 0;
}
