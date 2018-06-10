#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "socket.h"

//hostent definition:
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms738552(v=vs.85).aspx

int main(int argc, char const *argv[]){
    struct hostent *server; //colocar em socket
    struct sockaddr_in *addr_server = (struct sockaddr_in *)calloc(1,sizeof(struct sockaddr_in));
    char buffer[256];

    if((server = gethostbyname("172.26.146.8")) == NULL){
        printf("ERRO");
    }

    
    Socket s_client(9898, addr_server->sin_addr.s_addr, false);

    //sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //if(sockfd == -1){
    //    printf("erro");
    //    return -1;
    //}
    //
    //addr_server->sin_family = AF_INET;
    //bcopy((char *)server->h_addr, (char *)&addr_server->sin_addr.s_addr, server->h_length);
    //addr_server->sin_port = htons(portnb);
    //


    if(connect(s_client.getFdSocket(), (struct sockaddr *)s_client.getAddrSocket(), sizeof(struct sockaddr_in)) == -1){
        printf("Erro conncet(): %d", errno);
        return -1;        
    }

    //if(connect(sockfd, (struct sockaddr *)addr_server, sizeof(struct sockaddr))){
    //    printf("erro");
    //    return -1;
    //}

    writeMsg(s_client.getFdSocket(), s_client.buffer);
    //readMsg(s_client.getFdSocket(), s_client.buffer);

    //bzero(buffer, 256);

    //int n = write(sockfd, "oi", strlen("oi"));
    //printf("write: %d\n", n);

    //bzero(buffer, 256);

    //n = read(sockfd, buffer, 255);
    //printf("read: %d\n", n);

    //printf("buffer: %s\n", buffer);

    return 0;
}
