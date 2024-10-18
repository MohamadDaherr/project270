#include <stdio.h>
#include <stdlib.h>
#include <functions.c>

int main() {
  char grid1 [10][10];
  char grid2 [10][10];
    create_grid(grid1);
    create_grid(grid2);
    int difficulty = difficultyLevel();
    char player1[50], player2[50];  
    printf("Enter name of Player 1: ");
    scanf("%s", player1);
    printf("Enter name of Player 2: ");
    scanf("%s", player2);
    printf("%s please enter your ships",player1 );
    place_ships(grid1);
    print_Grid(grid1);
    printf("%s please enter your ships",player2);
    place_ships(grid2);
    print_Grid(grid2);
}
