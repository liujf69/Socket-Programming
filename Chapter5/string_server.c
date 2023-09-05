// gcc string_server.c -o string_server
// ./string_server 9190
 
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
 
int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
	    exit(1);
    }
  
    int Serv_sock = socket(PF_INET, SOCK_STREAM, 0); // create socket
    if (Serv_sock == -1) {
        ErrorHandling("socket() error");
    }
 
    struct sockaddr_in serv_addr; // allocate ip, port
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
 
    if (bind(Serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){ 
        ErrorHandling("bind() error");
    }
 
    if (listen(Serv_sock, 5) == -1) { // ready
        ErrorHandling("listen() error");
    }
 
    struct sockaddr_in clnt_addr;
    int clnt_addr_size = sizeof(clnt_addr);
 
    for (int i = 0; i < 5; ++i) {
        int Clnt_sock = accept(Serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);  // accept
        if (Clnt_sock == -1){
            ErrorHandling("accept() error");
        }
        else{
            printf("Connected client %d \n", i+1);
        }
 
        while(1) {
            int over = 0, recv_len = 0, msg_len, recv_cnt;
            char msg[BUF_SIZE];
            while (recv_len == 0 || recv_len < msg_len + Len_SIZE) {
                recv_cnt = read(Clnt_sock, (char*)&msg[recv_len], BUF_SIZE);
                if (recv_cnt == -1) {
                    ErrorHandling("read() error!");
                }
                if (recv_cnt == 0) {
                    over = 1;
                    break;
                }
                recv_len += recv_cnt;
                if (recv_len >= Len_SIZE) {
                    memcpy(&msg_len, msg, Len_SIZE);
                }
            }
            msg[recv_len] = '\0';
 
            if (over == 1) {
                break;
            }
 
            printf("Message from client: %s", msg + Len_SIZE); // output message from client
            fputs("Input message: ", stdout);
            fgets(msg + Len_SIZE, BUF_SIZE - Len_SIZE, stdin); // input message
            int len = strlen(msg + Len_SIZE);
            memcpy(msg, &len, Len_SIZE);
            write(Clnt_sock, msg, Len_SIZE + len); // send message to client
        }
        close(Clnt_sock);
    }
 
    close(Serv_sock);
    return 0;
}