// gcc string_client.c -o string_client
// ./string_client 127.0.1 9190
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#define BUF_SIZE 1024
#define Len_SIZE 4
 
void ErrorHandling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
 
int main(int argc, char* argv[]){
 
    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
	    exit(1);
    }
 
    int sock = socket(PF_INET, SOCK_STREAM, 0);   // create socket
    if (sock == -1)
        ErrorHandling("socket() error");
 
    struct sockaddr_in serv_addr; // allocate ip, port
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
 
    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){ // connect
        ErrorHandling("connect() error");
    }
    else {
        puts("Connected");
    }
 
    for(int i = 0; i < 3; i++) {
        char msg[BUF_SIZE];
        memset(msg, 0, BUF_SIZE);
        
        fputs("Input message: ", stdout);
        fgets(msg + Len_SIZE, BUF_SIZE - Len_SIZE, stdin); // input message
        int len = strlen(msg + Len_SIZE);
        memcpy(msg, &len, Len_SIZE);
        write(sock, msg, Len_SIZE + len); // send message to server
 
        int recv_len = 0, msg_len = 0, recv_cnt = 0;
        while (recv_len == 0 || recv_len < msg_len + Len_SIZE) {
            recv_cnt = read(sock, (char*)&msg, BUF_SIZE);
            recv_len += recv_cnt;
            if (recv_len >= Len_SIZE) {
                memcpy(&msg_len, msg, Len_SIZE);
            }
        }
        msg[recv_len] = '\0';
        printf("Message from server: %s", msg + Len_SIZE);
    }
 
    close(sock);
    return 0;
}