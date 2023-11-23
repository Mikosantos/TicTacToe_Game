/*  TO COMPILE:
    gcc tictactoe.c -Wall -o run.exe
*/

#include <stdio.h>
#include <stdlib.h>
void 
printColor(int nColor) 
{
	switch (nColor) 
	{
		case 2: printf("\e[0;32m"); break;	// Dark green
		case 3: printf("\e[0;33m"); break;	// Dark yellow
		case 5: printf("\e[0;36m"); break;	// Dark blue
		case 9: printf("\e[0;91m"); break;	// Bright red
		case 10: printf("\e[0;36m"); break;	// Lime
		case 13: printf("\e[0;95m"); break;	// Magenta
		case 14: printf("\e[4;36m"); break;	// Cyan
		case 15: printf("\e[0;97m"); break;	// White
	}
}

int used_pegs[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
void printgame(int game[][3])
{
    printColor(15); printf("==========================\n");    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("|   %d\t ", game[i][j]);
        }
        printf("|\n");
    }
    printColor(15); printf("==========================\n");    
}
void inputPoint(int game[][3], int row, int column, int peg)
{
    game[row - 1][column - 1] = peg;
}

/*  Checks if a peg has been used, then adds said peg to the used pegs list
    @param game[][]         The current game state
    @param peg              The peg to check
    @param used_pegs[]      The array of used pegs
    @return                 Boolean (1/0) on whether the space is occupied
*/
int checkPegs(int game[][3], int peg, int used_pegs[])
{
    if (used_pegs[peg - 1] != 0){
        printColor(9); printf("\n>> Peg %d has already been used. Try again. <<\n\n", peg);
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game[i][j] == peg) {
                printColor(9); printf("\n>> Peg %d is already on the board. Try again. <<\n\n", peg);
                return 0;
            }
        }
    }

    return 1;
}

/*  Checks the winner of a game.
    @param game             The current game state
    @param player           The current player.
    @return                 -1 if neither player has won, non-0 if either has
                            0 if player 0 has won
                            1 if player 1 has won
                            2 if a tie occurs

*/
int checkWinner (int game[][3], int player)
{
    // initialization
    int i, j;
    int totalSums[6] = {0, 0, 0, 0, 0, 0}; // T, M, B, L, C, R
    int totalPegs[6] = {0, 0, 0, 0, 0, 0};
    int totalSumIndex = 0;
    int perfectGame = 1; // used to check if all rows and colums are still complete and sum to 15
    int ret = -1; // value to be returned

    char winner2 = 'A' + (player + 1) % 2; // opposing player
    char winner1 = 'A' + player % 2; // current player

    // compute columns
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            // increment sums and total pegs if applicable
            if (game[j][i] > 0) {
                totalSums[totalSumIndex] += game[j][i];
                totalPegs[totalSumIndex] += 1;
            }
        }
        totalSumIndex++;
    }
    // compute rows
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            // increment sums and total pegs if applicable
            if (game[i][j] > 0) {
                totalSums[totalSumIndex] += game[i][j];
                totalPegs[totalSumIndex] += 1;
            }
        }
        totalSumIndex++;
    }
    // check for game over conditions
    for (i = 0; i < 6; i++) {

        // LOSE CONDITION:
        // if for any complete row/col, the total sum < 15, player loses
        if (totalPegs[i] == 3 && totalSums[i] < 15) {
            printColor(2); printf("\nPlayer %c wins!\n", winner2); printColor(15);
            ret = !player;
            perfectGame = 0;
            break;
        }

        // WIN CONDITION:
        // if all rows and cols are complete /and/ their total sums = 15, player wins
        // this checks if this doesn't happen
        else if (totalPegs[i] != 3 || totalSums[i] != 15) {
            perfectGame = 0;
        }
    }
    // WIN CONDITION
    // triggers when it's still up by the time this happens
    if (perfectGame == 1) {
        printColor(2); printf("\nPlayer %c wins!\n", winner1); printColor(15);
        ret = player;
    }
    return ret;

}

/*  Lets the user input moves.
    @param *peg             Edited to contain the peg used
    @param *row             Edited to contain the player's row
    @param *column          Edited to contain the player's column
    @param player           The player number
    @param game[][]         The game state
    @return                 Always returns 1
*/
int inputPlayerInfo(int *peg, int *row, int *column, int player, int game[][3])
{
    int pegValid = 0, posValid = 0;
    printColor(15);printf("|");
    printColor(13); printf("    Player %c's turn:    ", 'A' + player%2);
    printColor(15); printf("|\n==========================\n");  
    // require both peg and pos satisfied
    while (pegValid * posValid == 0) {
        // sanitize inputs
        fflush(stdin);

        // get inputs
        if (pegValid == 0) {
            printColor(15); printf("Enter peg (1-9): ");
            printColor(5); scanf("%d", peg);
        }

        if (posValid == 0) {
            printColor(15); printf("Enter row (1-3): ");
            printColor(5); scanf("%d", row);
            printColor(15); printf("Enter column (1-3): ");
            printColor(5); scanf("%d", column);
        }

        // check if peg is valid
        pegValid = checkPegs(game, *peg, used_pegs);

        // check if position is valid
        if (0 < *row && *row < 4 && 0 < *column && *column < 4) {
            if (game[*row-1][*column-1] == 0)
                posValid = 1;
            else{
                printColor(9); printf("\n>> That space is already taken. Try again. <<\n\n");
            }
        }
        else{
            printColor(9); printf("\n>> That space is out of bounds. Try again. <<\n\n");
        }

    }

    return 1;
}

/*  This is the main.
    It does not have any parameters and should always return 0.
*/
int main()
{
    int game[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    int player = 1, row, column, peg;

    // main game loop
    while (1) {
        // print game state
        system("cls");
        printColor(15); printf("==========================\n|");   
        printColor(3); printf("   Let the Game Begin!  ");        
        printColor(15); printf("|\n|");   
        printColor(3); printf("   Ready that Square!   ");   
        printColor(15); printf("|\n"); printgame(game);
        // input move
        inputPlayerInfo(&peg, &row, &column, player+1, game);
        used_pegs[peg-1] = 1;
        inputPoint(game, row, column, peg);

        // check winner, display final state if there is a winner
        if (checkWinner(game, player+1) != -1){
            printgame(game);
            break;
        }
        // pass control to next player
        player = !player;

    }

    return 0;
}
