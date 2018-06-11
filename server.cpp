#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "socket.h"
#include "tictactoe.h"

int main(int argc, char const *argv[]){
    Socket socket(9898, INADDR_ANY, true);
    socket.acceptClients(1);

    printf("Server é o jogador 2\n");

    int player = 0,i,choice;
	char mark;

	do{
		board();
		cout << "Player " << player + 1 << ", enter a number:  ";
		if(player == 1){
            printf("Sua vez");
            writeMsg(socket.getFdClients()[0], socket.buffer);
            choice = atoi(socket.buffer);
        }else{
            printf("Esperando jogador");
            readMsg(socket.getFdClients()[0], socket.buffer);
            choice = atoi(socket.buffer);
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
   
    socket.closeSocket();

    return 0;
}
