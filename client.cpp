#include <stdlib.h>
#include <stdio.h>

#include "socket.h"

//hostent definition:
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms738552(v=vs.85).aspx

int main(int argc, char const *argv[]){
    int portnb = 9898;
    struct hostent *server; //colocar em socket
    

    if((server = gethostbyname("172.26.146.8")) == NULL){
        printf("ERRO");
    }




    


    return 0;
}
