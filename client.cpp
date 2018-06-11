#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "socket.h"

//hostent definition:
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms738552(v=vs.85).aspx


int main(int argc, char const *argv[]){
    if(argc != 3){
        printf("Missing arguments\n");
        exit(EXIT_FAILURE);
    }
    
    struct hostent *server;
    struct sockaddr_in *addr_server = (struct sockaddr_in *)calloc(1,sizeof(struct sockaddr_in));

    if((server = gethostbyname(argv[2])) == NULL){
        printf("ERRO");
    }

    Socket s_client(argv[1], addr_server->sin_addr.s_addr, false);

    if(connect(s_client.getFdSocket(), (struct sockaddr *)s_client.getAddrSocket(), sizeof(struct sockaddr_in)) == -1){
        printf("Erro conncet(): %d", errno);
        return -1;
    }

    while(1){
        writeMsg(s_client.getFdSocket(), s_client.buffer);
        //sleep(1);
        readMsg(s_client.getFdSocket(), s_client.buffer);
    }
    return 0;
}
