using namespace std;


/* FUNCTION TO DRAW BOARD OF TIC TAC TOE WITH PLAYERS MARK */
void board(char square[]);

/* Function that checks if the game is over
   returns:
		1 if somebody won
		0 if it was a draw
		-1 if it's not over yet
*/
int check(char square[]);