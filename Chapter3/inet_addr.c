// gcc inet_addr.c -o inet_addr
// ./inet_addr

#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    char *arrd1 = "1.2.3.4"; 
    char *arrd2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(arrd1);
    if(conv_addr == INADDR_NONE){
        printf("Error occured \n");
    }
    else{
        printf("Network ordered integer addr: %#lx \n", conv_addr);
    }

    conv_addr = inet_addr(arrd2);
    if(conv_addr == INADDR_NONE){
        printf("Error occured \n");
    }
    else{
        printf("Network ordered integer addr: %#lx \n", conv_addr);
    }

    return 0;
}