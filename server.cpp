#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>

#include "socket.h"
#include "tictactoe.h"

/* Board matrix */
char square[10] = {'o','1','2','3','4','5','6','7','8','9'};

int main(int argc, char const *argv[]){    
    if(argc != 2){
        printf("argc: %d\tMissing PORT number\n", argc);
        exit(EXIT_FAILURE);
    }
    
    Socket socket(argv[1], INADDR_ANY, true);
    socket.acceptClients(1);

	printf("Server é o jogador 2\n");

    int player = 0,i,choice;
	char mark;

	do{
		board(square);
		if(player == 1){
            printf("Sua vez digite uma posição\n");
            writeMsg(socket.getFdClients()[0], &socket.buffer);
            choice = atoi(&socket.buffer);
		}else{
            printf("Esperando vez do outro jogador...\n");
            readMsg(socket.getFdClients()[0], &socket.buffer);
            choice = atoi(&socket.buffer);
		}

		mark = (!player) ? 'X' : 'O'; 

		if(isdigit(square[choice])){
			square[choice] = mark;
		}else{
			player--;
			cout<<"ERROR: Invalid Position";
			/* Waiting for enter */
			scanf("%*c");
		}
		player = (player+1) % 2;
		i = check(square);
		
	}while(i == -1);
	
	board(square);
	
	i == 1 ? cout<<"\aPlayer: "<<(++player)%2 + 1<<" WON!!!\n" : cout<<"\aIts a draw!!!";

	/* Enter to quit */
	scanf("%*c");
    
    socket.closeSocket();

    return 0;
}
