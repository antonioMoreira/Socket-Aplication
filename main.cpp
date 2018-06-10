#include <stdlib.h>
#include <stdio.h>

#include "socket.h"


int main(int argc, char const *argv[]){
    Socket socket(9898,1);
    
    getInformation(socket.getFdSocket());
    socket.acceptClients();

    writeMsg(socket.getFdSocket(), socket.buffer);

    


    socket.closeSocket();

    return 0;
}
