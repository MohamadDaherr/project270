#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define ship 'S'
#define water '~'
#define Grid_size 10
#define hit '*'
#define miss 'o'

#define Carrier 5
#define Battleship 4
#define Destroyer 3
#define Submarine 2

int diff;

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
int traverse_2d_array(char grid[10][10], int letter) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == letter) {
                return 1;
        }
    }
}
return 0;
}

void print_Grid(char grid[10][10]){
    // Display the column headers (A-J)
    printf("   A B C D E F G H I J\n");
    

    // Display the grid with row numbers (1-10).
    for (int i = 0; i < 10; i++) {
        printf("%2d ", i + 1); 
        for (int j = 0; j < 10; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}

void create_grid(char grid[10][10]) {

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = water;
        }
    }
}


void difficultyLevel() {
    char difficulty[10];
    
    while (1) {  
        printf("Choose difficulty level (Easy or Hard): ");
        scanf("%s", difficulty);
        // convert it to lowercase because it is case insensitive
        for (int i = 0; difficulty[i]; i++) {
            difficulty[i] = tolower(difficulty[i]);
        }

        if (strcmp(difficulty, "easy") == 0) {
            diff=0;
            printf("You have chosen easy mode.\n");
            break;
        } else if (strcmp(difficulty, "hard") == 0) {
            diff=1;
            printf("You have chosen hard mode.\n");
            break;
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

void place_ships(char grid[10][10]){
    for (int i = 0; i < total_ships; i++)
    {
        int row,col;
        char orientation;
        int valid =1;

        while(valid)
        {
            printf("Place your %s (size %d).\n",ships[i].name,ships[i].size);
            printf("Enter column (A_J), row (1-10), and orientation (H for horizontal, V for vertical): ");
            
            //This variable to convert from letter to the exact index
            char column;
            scanf(" %c %d %c", &column, &row, &orientation);          
            col = toupper(column) -'A';
            row--;

            if(check_coordinates(row,col,ships[i].size,orientation,grid)){
                for (int j = 0; j < ships[i].size; j++)
                {
                    if(orientation =='H' || orientation =='h'){
                        grid[row][col + j] = ships[i].name[0];
                    }
                    else{
                        grid[row+j][col]= ships[i].name[0];
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

int firstPlayer(char player1[], char player2[]) {
  srand(time(NULL));
    int first_player = rand() % 2;  
    if (first_player == 0) {
        printf("%s gets the first move!\n", player1);
        return 1;
    } else {
        printf("%s gets the first move!\n", player2);
        return 2;
    }
}



int done() {
    char input;
    // Loop until the user enters 'd'
    while (1) {
        printf("Enter 'd' when you are done: ");
        scanf(" %c", &input);  
        input = tolower(input);
        if (input == 'd') {
            printf("Thank you! You entered 'd'.\n");
            return 1;
        } else {
            printf("Invalid input. Please enter 'd' to indicate you are done.\n");
        }
    }
}

int validate(int row , int col){
if (col < 0 || col >= Grid_size || row < 0 || row >= Grid_size) {
        printf("Invalid coordinates.");
        return 0;
    }
    return 1;
}

void fire(char grid[10][10] , char grid2[10][10], int row ,char column) {
    int col;
    col = toupper(column) - 'A';
    row--;

    if (validate(row , col)){
        if (grid[row][col] == 'C' || grid[row][col] == 'D' || grid[row][col] == 'S' || grid[row][col] == 'B') {
            grid[row][col] = hit;  
            grid2[row][col] = hit;
            printf("Hit!\n");

        }
        else {
            if(diff==0){
             grid[row][col]=miss;
             grid2[row][col] = miss;
                printf("Miss!\n");
            }
        }
    }
    else {
       return;
    }
}
    
void radar_sweep(int row,char column,char grid[10][10]){
    int col;
    col = toupper(column) - 'A';
    if(!validate(row,col)){
        return;
    }
    int found = 0;
    for (int i = row; i < row + 2 && i<Grid_size; i++) {
        for (int j = col; j < col + 2 && j<Grid_size; j++) {
            if (grid[i][j] == 'C' || grid[i][j] == 'D' || grid[i][j] == 'S' || grid[i][j] == 'B') {
                found = 1;
            }
        }
    }
    if (found) {
        printf("Enemy ships found in the area.\n");
    } else {
        printf("No enemy ships found.\n");
    }
}
void torpedo(char grid[10][10], char grid2[10][10], char target) {
    int is_column = isalpha(target);
    
    if (is_column) {
        int col = toupper(target) - 'A'; 

        // Validate the column
        if (col < 0 || col >= Grid_size) {
            printf("Invalid column. You lose your turn.\n");
            return;
        }

        // Fire at each row in the specified column
        for (int row = 0; row < Grid_size; row++) {
            fire(grid, grid2, row + 1, target); // Pass row as 1-based index
        }
    } else {
        // Convert target to row number
        int row = target - '1'; 

        // Validate the row
        if (row < 0 || row >= Grid_size) {
            printf("Invalid row. You lose your turn.\n");
            return;
        }

        // Fire at each column in the specified row
        for (int col = 0; col < Grid_size; col++) {
            fire(grid, grid2, row + 1, col + 'A'); // Pass col as character
        }
    }
}


        
 
int main() {
  char grid1 [10][10];
  char grid2 [10][10];
  char gridplayer1 [10][10];
  char gridplayer2 [10][10];
    create_grid(grid1);
    create_grid(grid2);
    create_grid(gridplayer1);
    create_grid(gridplayer2);
    difficultyLevel();
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
  fire(grid1,gridplayer1,2,'B');
  print_Grid(gridplayer1);
  torpedo(grid1,gridplayer1,'A');




   
}