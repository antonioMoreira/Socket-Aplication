#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <strings.h>

#include "socket.h"
#include "tictactoe.h"

//hostent definition:
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms738552(v=vs.85).aspx

int main(int argc, char const *argv[]){
    struct hostent *server; //colocar em socket
    struct sockaddr_in *addr_server = (struct sockaddr_in *)calloc(1,sizeof(struct sockaddr_in));

    if((server = gethostbyname("172.26.146.8")) == NULL){
        printf("ERRO");
    }

    Socket s_client(9898, addr_server->sin_addr.s_addr, false);

    if(connect(s_client.getFdSocket(), (struct sockaddr *)s_client.getAddrSocket(), sizeof(struct sockaddr_in)) == -1){
        printf("Erro conncet(): %d", errno);
        return -1;        
    }
    
    int player = 0,i,choice;
	char mark;

	do{
		board();
		cout << "Player " << player + 1 << ", enter a number:  ";
		if(player == 0){
            printf("Sua vez");
            writeMsg(s_client.getFdSocket(), s_client.buffer);
            choice = atoi(s_client.buffer);
        }else{
            printf("Esperando jogador");
            readMsg(s_client.getFdSocket(), s_client.buffer);
            choice = atoi(s_client.buffer);
        }

		mark = (!player) ? 'X' : 'O'; 

		if(isdigit(square[choice])){
			square[choice] = mark;
		}else{
			player--;
			cout<<"ERROR: Invalid Position";
			/* Waiting for enter */
			cin.ignore();
			cin.get();
		}
		player = (player+1) % 2;
		i = check();
		
	}while(i == -1);
	
	board();
	
	i == 1 ? cout<<"\aPlayer: "<<(++player)%2 + 1<<" WON!!!" : cout<<"\aIts a draw!!!";

	/* Enter to quit */
	cin.ignore();
	cin.get();

    return 0;
}
