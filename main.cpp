#include <stdlib.h>
#include <stdio.h>

#include "socket.h"


int main(int argc, char const *argv[]){
    Socket socket(9898, INADDR_ANY, true);
    socket.acceptClients(1);

    readMsg(socket.getFdClients()[0], socket.buffer);
    

    //writeMsg(socket.getFdClients()[0], socket.buffer);

    socket.closeSocket();

    return 0;
}
