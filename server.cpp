#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "socket.h"
#include "tictactoe.h"

/* Board matrix */
char square[10] = {'o','1','2','3','4','5','6','7','8','9'};

int main(int argc, char const *argv[]){
    Socket socket(9898, INADDR_ANY, true);
    socket.acceptClients(1);

    printf("Server é o jogador 2\n");

    int player = 0,i,choice;
	char mark;

	do{
		board(square);
		cout << "Player " << player + 1 << ", enter a number:  \n";
		if(player == 1){
            printf("Sua vez\n");
            writeMsg(socket.getFdClients()[0], socket.buffer);
            choice = atoi(socket.buffer);
			printf("Depois do write do server : %s", socket.buffer);
			cin.ignore();
			cin.get();
			
		}else{
            printf("Esperando jogador\n");
            choice = readMsg(socket.getFdClients()[0], socket.buffer);
            printf("Depois do read no server : choice = %d buff = %s", choice,socket.buffer);
			cin.ignore();
			cin.get();
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
		i = check(square);
		
	}while(i == -1);
	
	board(square);
	
	i == 1 ? cout<<"\aPlayer: "<<(++player)%2 + 1<<" WON!!!" : cout<<"\aIts a draw!!!";

	/* Enter to quit */
	cin.ignore();
	cin.get();
   
    socket.closeSocket();

    return 0;
}
