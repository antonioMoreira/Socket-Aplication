#include <stdlib.h>
#include <stdio.h>

#include "socket.h"


int main(int argc, char const *argv[]){
    Socket socket(9898, INADDR_ANY, true);
    socket.acceptClients(5);

    readMsg(socket.getFdSocket(), socket.buffer);
    
    //writeMsg(socket.getFdSocket(), socket.buffer);

    socket.closeSocket();

    return 0;
}
