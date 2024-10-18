#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#define water '~'

void create_grid() {
    // Initialize a 10x10 grid with '~' representing water
    char grid[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = water;
        }
    }
    // Display the column headers (A-J)
    printf("   A B C D E F G H I J\n");
    
    // Display the grid with row numbers (1-10)
    for (int i = 0; i < 10; i++) {
        printf("%2d ", i + 1); 
        for (int j = 0; j < 10; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}

int difficultyLevel() {
    char difficulty[10];
    
    while (1) {  
        printf("Choose difficulty level (Easy or Hard): ");
        scanf("%s", difficulty);
        // convert it to lowercase because it is case insensitive
        for (int i = 0; difficulty[i]; i++) {
            difficulty[i] = tolower(difficulty[i]);
        }

        if (strcmp(difficulty, "easy") == 0) {
            printf("You have chosen easy mode.\n");
            return 1; 
        } else if (strcmp(difficulty, "hard") == 0) {
            printf("You have chosen hard mode.\n");
            return 2;
        } else {
            printf("Invalid input. Please enter either 'easy' or 'hard'.\n");// Reask the user to enter a valid level
        }
    }
}

void playerNames(char player1[], char player2[]) {
    printf("Enter name of Player 1: ");
    scanf("%s", player1);

    printf("Enter name of Player 2: ");
    scanf("%s", player2);
}

void firstPlayer(char player1[], char player2[]) {
  srand(time(NULL));
    int first_player = rand() % 2;  
    if (first_player == 0) {
        printf("%s gets the first move!\n", player1);
    } else {
        printf("%s gets the first move!\n", player2);
    }
}
