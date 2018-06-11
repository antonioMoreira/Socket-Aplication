using namespace std;

/* Board matrix */
char square[10] = {'o','1','2','3','4','5','6','7','8','9'};

/* FUNCTION TO DRAW BOARD OF TIC TAC TOE WITH PLAYERS MARK */
void board();

/* Function that checks if the game is over
   returns:
		1 if somebody won
		0 if it was a draw
		-1 if it's not over yet
*/
int check();