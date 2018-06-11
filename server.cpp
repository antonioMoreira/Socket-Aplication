#include <stdlib.h>
#include <stdio.h>
#include <ctime>

#include "socket.h"


int main(int argc, char const *argv[]){    
    if(argc != 2){
        printf("argc: %d\tMissing PORT\n", argc);
        exit(EXIT_FAILURE);
    }

    printf("Port: %s\n ", argv[1]);
    
    Socket socket(argv[1], INADDR_ANY, true);
    socket.acceptClients(1);

    while(1){
        sleep(1);
        readMsg(socket.getFdClients()[0], socket.buffer);
        writeMsg(socket.getFdClients()[0], socket.buffer);
    }

    socket.closeSocket();

    return 0;
}
