#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "tictactoe.h"

using namespace std;

/* FUNCTION TO DRAW BOARD OF TIC TAC TOE WITH PLAYERS MARK */
void board(char square[]){
	system("clear");
	cout << "\n\n\tTic Tac Toe\n\n";

	cout << "Player 1 (X)  -  Player 2 (O)\n\n\n";
	cout << "     |     |     \n";
	cout << "  " << square[1] << "  |  " << square[2] << "  |  " << square[3] << endl;
	cout << "_____|_____|_____\n";
	cout << "     |     |     \n";
	cout << "  " << square[4] << "  |  " << square[5] << "  |  " << square[6] << endl;
	cout << "_____|_____|_____\n";
	cout << "     |     |     \n";
	cout << "  " << square[7] << "  |  " << square[8] << "  |  " << square[9] << endl;
	cout << "     |     |     \n\n";
}

/* Function that checks if the game is over
   returns:
		1 if somebody won
		0 if it was a draw
		-1 if it's not over yet
*/
int check(char square[]){
	if (square[1] == square[2] && square[2] == square[3]) return 1;
	else if (square[4] == square[5] && square[5] == square[6]) return 1;
	else if (square[7] == square[8] && square[8] == square[9]) return 1;
	else if (square[1] == square[4] && square[4] == square[7]) return 1;
	else if (square[2] == square[5] && square[5] == square[8]) return 1;
	else if (square[3] == square[6] && square[6] == square[9]) return 1;
	else if (square[1] == square[5] && square[5] == square[9]) return 1;
	else if (square[3] == square[5] && square[5] == square[7]) return 1;
	else if (square[1] != '1' && square[2] != '2' && square[3] != '3' && 
			 square[4] != '4' && square[5] != '5' && square[6] != '6' && 
			 square[7] != '7' && square[8] != '8' && square[9] != '9') return 0;
	else return -1;
}