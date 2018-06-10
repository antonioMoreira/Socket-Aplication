#include <stdlib.h>
#include <stdio.h>

#include "socket.h"


int main(int argc, char const *argv[]){
    Socket socket(9897, true);
    socket.acceptClients(1);

    readMsg(socket.getFdSocket(), socket.buffer);
    
    writeMsg(socket.getFdSocket(), socket.buffer);

    socket.closeSocket();

    return 0;
}
