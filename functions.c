#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define ship 'S'
#define water '~'
#define Grid_size 10

#define Carrier 5
#define Battleship 4
#define Destroyer 3
#define Submarine 2

typedef struct {
    char *name;
    int size;
} Ship;

Ship ships[] = {
    {"Carrier", Carrier},
    {"Battleship", Battleship},
    {"Destroyer", Destroyer},
    {"Submarine", Submarine},
};

int total_ships = sizeof(ships) / sizeof(ships[0]);

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

int check_coordinates(int row , int col, int size, char orientation, char grid[Grid_size][Grid_size]){
    if(orientation=='H' || orientation=='h'){

        //This base case to check if a ship goes ou of bounds
        if(col+size>Grid_size)
            return 0;
        
        for (int i = 0; i < size; i++) {
            if (grid[row][col + i] != water) return 0;  
        }
    }
    else if(orientation=='V' || orientation=='v'){
        //This base case to check if a ship goes ou of bounds
        if(row+size>Grid_size)
            return 0;
        
        for (int i = 0; i < size; i++) {
            if (grid[row+i][col] != water) return 0;  
        }
    }
    return 1;
}

void place_ships(char grid[Grid_size][Grid_size]){
    for (int i = 0; i < total_ships; i++)
    {
        int row,col;
        char orientation;
        int valid =1;

        while (valid)
        {
            printf("Place your %s (size %d).\n");
            printf("Enter column (A_J), row (1-10), and orientation (H for horizontal, V for vertical): ",ships[i].name,ships[i].size);
            
            //This variable to convert from letter to the exact index
            char column;
            scan_f("%c %d %c", &column, &row, &orientation);          
            col = column -'A';
            row--;

            if(check_coordinates(row,col,ships[i].size,orientation,grid)){
                for (int i = 0; i < ships[i].size; i++)
                {
                    if(orientation=='H' || orientation=='h'){
                        grid[row][col + i] = ship;
                    }
                    else{
                        grid[row+i][col]= ship;
                    }
                }
                valid =0;
            }  
            else{
                printf("Invalid Placement. Try again\n");
            }
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
