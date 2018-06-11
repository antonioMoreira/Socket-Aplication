#include <stdlib.h>
#include <stdio.h>
#include <ctime>

#include "socket.h"


int main(int argc, char const *argv[]){    
    if(argc != 2){
        printf("Missing PORT");
        exit(EXIT_FAILURE);
    }
    
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
