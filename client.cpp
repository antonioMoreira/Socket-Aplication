#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <strings.h>

#include "socket.h"
#include "tictactoe.h"

//hostent definition:
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms738552(v=vs.85).aspx

/* Board matrix */
char square[10] = {'o','1','2','3','4','5','6','7','8','9'};

int main(int argc, char const *argv[]){
    if(argc != 3){
        printf("Missing arguments\n");
        exit(EXIT_FAILURE);
    }
    
    struct hostent *server;
    struct sockaddr_in *addr_server = (struct sockaddr_in *)calloc(1,sizeof(struct sockaddr_in));

    if((server = gethostbyname(argv[2])) == NULL){
        printf("ERRO em gethostbyname()");
    }

    Socket s_client(argv[1], addr_server->sin_addr.s_addr, false);

    if(connect(s_client.getFdSocket(), (struct sockaddr *)s_client.getAddrSocket(), sizeof(struct sockaddr_in)) == -1){
        printf("Erro conncet(): %d", errno);
        return -1;
    }

    int player = 0,i,choice;
	char mark;

	do{
		board(square);
		//cout << "Player " << player + 1 << ", enter a number:  \n";
		mark = (!player) ? 'X' : 'O'; 

		if(player == 0){
            printf("Sua vez digite uma posição\n");
            writeMsg(s_client.getFdSocket(), &s_client.buffer);
            choice = atoi(&s_client.buffer);
		}else{
            printf("Esperando jogador...\n");
            readMsg(s_client.getFdSocket(), &s_client.buffer);
			choice = atoi(&s_client.buffer);
		}

		
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

    return 0;
}
