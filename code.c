#include <stdio.h>
#include <stdlib.h>
#include <functions.c>

int main() {
    create_grid();
    int difficulty = difficultyLevel();
    char player1[50], player2[50];  
    playerNames(player1, player2);
    firstPlayer(player1, player2);

}
