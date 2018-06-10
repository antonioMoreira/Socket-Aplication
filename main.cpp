#include <stdlib.h>
#include <stdio.h>

#include "socket.h"


int main(int argc, char const *argv[]){
    Socket socket(55555,2);
    
    getInformation(socket.getFdSocket());
    socket.acceptClients();
    socket.Oi();


    int a = 0;
    while(a != 1){
        printf("%d", a);
        scanf("%d", &a);
    }

    socket.closeSocket();

    return 0;
}
