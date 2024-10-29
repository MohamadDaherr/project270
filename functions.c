#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include "function_headers.h"


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
int currentPlayer;

typedef struct
{
    char *name;
    int size;
} Ship;

Ship ships[] = {
    {"Carrier", Carrier},
    {"Battleship", Battleship},
    {"Destroyer", Destroyer},
    {"Submarine", Submarine},
};

char available_ships1[] = {'C', 'B', 'D', 'S'};
char available_ships2[] = {'C', 'B', 'D', 'S'};

int total_ships = sizeof(ships) / sizeof(ships[0]);

int contains(char available_ships[], char letter)
{
    for (int i = 0; i < total_ships; i++)
    {
        if (available_ships[i] == letter)
        {
            available_ships[i] = tolower(letter);
            return 1;
        }
    }
    return 0;
}

int sunk_ships(char grid[10][10])
{
    int count = 0;

    int countc = 0, countb = 0, countd = 0, counts = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            char c = toupper(grid[i][j]);
            switch (c)
            {
            case 'C':
                countc++;
                break;

            case 'B':
                countb++;
                break;

            case 'D':
                countd++;
                break;
            case 'S':
                counts++;
                break;
            }
        }
    }

    if (countc == 0)
    {
        if (currentPlayer == 1)
        {
            if (contains(available_ships2, 'C') == 1)
            {
                printf("Carrier ship sunk!\n");
                count++;
            }
        }
        else
        {
            if (contains(available_ships1, 'C') == 1)
            {
                printf("Carrier ship sunk!\n");
                count++;
            }
        }
    }
    if (countb == 0)
    {
        if (currentPlayer == 1)
        {
            if (contains(available_ships1, 'B') == 1)
            {
                printf("Battleship ship sunk!\n");
                count++;
            }
        }
        else
        {
            if (contains(available_ships2, 'B') == 1)
            {
                printf("Battleship ship sunk!\n");
                count++;
            }
        }
    }
    if (countd == 0)
    {
        if (currentPlayer == 1)
        {
            if (contains(available_ships1, 'D') == 1)
            {
                printf("Destroyer ship sunk!\n");
                count++;
            }
        }
        else
        {
            if (contains(available_ships2, 'D') == 1)
            {
                printf("Destroyer ship sunk!\n");
                count++;
            }
        }
    }
    if (counts == 0)
    {
        if (currentPlayer == 1)
        {
            if (contains(available_ships1, 'S') == 1)
            {
                printf("Submarine ship sunk!\n");
                count++;
            }
        }
        else
        {
            if (contains(available_ships2, 'S') == 1)
            {
                printf("Submarine ship sunk!\n");
                count++;
            }
        }
    }

    return count;
}

int traverse_2d_array(char grid[10][10], int letter)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (grid[i][j] == letter)
            {
                return 1;
            }
        }
    }
    return 0;
}

void print_Grid(char grid[10][10])
{
    // Display the column headers (A-J)
    printf("   A B C D E F G H I J\n");

    // Display the grid with row numbers (1-10).
    for (int i = 0; i < 10; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < 10; j++)
        {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}

void create_grid(char grid[10][10])
{

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            grid[i][j] = water;
        }
    }
}

void difficultyLevel()
{
    char difficulty[10];

    while (1)
    {
        printf("Choose difficulty level (Easy or Hard): ");
        scanf("%s", difficulty);
        // convert it to lowercase because it is case insensitive
        for (int i = 0; difficulty[i]; i++)
        {
            difficulty[i] = tolower(difficulty[i]);
        }

        if (strcmp(difficulty, "easy") == 0)
        {

            diff = 0;
            printf("You have chosen easy mode.\n");
            break;
        }
        else if (strcmp(difficulty, "hard") == 0)
        {
            diff = 1;
            printf("You have chosen hard mode.\n");
            break;
        }
        else
        {
            printf("Invalid input. Please enter either 'easy' or 'hard'.\n"); // Reask the user to enter a valid level
        }
    }
}

int check_coordinates(int row, int col, int size, char orientation, char grid[Grid_size][Grid_size])
{
    if (orientation == 'H' || orientation == 'h')
    {

        // This base case to check if a ship goes ou of bounds
        if (col + size > Grid_size)
            return 0;

        for (int i = 0; i < size; i++)
        {
            if (grid[row][col + i] != water)
                return 0;
        }
    }
    else if (orientation == 'V' || orientation == 'v')
    {
        // This base case to check if a ship goes ou of bounds
        if (row + size > Grid_size)
            return 0;

        for (int i = 0; i < size; i++)
        {
            if (grid[row + i][col] != water)
                return 0;
        }
    }
    else
    {
        return 0;
    }
    return 1;
}
void place_ships(char grid[10][10])
{

    for (int i = 0; i < total_ships; i++)
    {
        int row, col;
        char orientation;
        int valid = 1;
        char column;
        while (valid)
        {
            printf("Place your %s (size %d).\n", ships[i].name, ships[i].size);
            printf("Enter column (A_J), row (1-10), and orientation (H for horizontal, V for vertical): ");
            // scanf(" %c %d %c", &column, &row, &orientation);
            if (scanf(" %c %d %c", &column, &row, &orientation) != 3)
            {
                printf("Invalid input format. Try again.\n");
                // Clear input buffer
                while (getchar() != '\n')
                    ;
                continue; // Go back to the beginning of the loop
            }
            if (!isalpha(column))
            {
                printf("Invalid input format. Try again.\n");
                continue; // Skip to the next iteration
            }
            col = toupper(column) - 'A';
            row--;
            if (check_coordinates(row, col, ships[i].size, orientation, grid))
            {
                for (int j = 0; j < ships[i].size; j++)
                {
                    if (orientation == 'H' || orientation == 'h')
                    {
                        grid[row][col + j] = ships[i].name[0];
                    }
                    else if (orientation == 'V' || orientation == 'v')
                    {
                        grid[row + j][col] = ships[i].name[0];
                    }
                }
                valid = 0;
            }
            else
            {
                printf("Invalid Placement. Try again\n");
            }
        }
    }
}
int firstPlayer(char player1[], char player2[])
{
    srand(time(NULL));
    int first_player = rand() % 2;
    if (first_player == 0)
    {
        printf("%s gets the first move!\n", player1);
        return 1;
    }
    else
    {
        printf("%s gets the first move!\n", player2);
        return 2;
    }
}
int validate(int row, int col)
{
    if (col < 0 || col >= Grid_size || row < 0 || row >= Grid_size)
    {
        printf("ERROR out of bounds \n");
        return 0;
    }
    return 1;
}

void fire(char grid[10][10], char grid2[10][10], int row, char column)
{
    int col;
    col = toupper(column) - 'A';
    row--;

    if (validate(row, col))
    {
        if (toupper(grid[row][col]) == 'C' || toupper(grid[row][col]) == 'D' || toupper(grid[row][col]) == 'S' || toupper(grid[row][col]) == 'B' || grid[row][col] == '*')
        {
            grid[row][col] = hit;
            grid2[row][col] = hit;
        }
        else
        {
            if (diff == 0)
            {
                grid[row][col] = miss;
                grid2[row][col] = miss;
            }
        }
    }
    else
    {
        return;
    }
}

void radar_sweep(int row, char column, char grid[10][10])
{
    int col;
    row--;
    col = toupper(column) - 'A';
    if (!validate(row, col))
    {
        return;
    }
    int found = 0;
    for (int i = row; i < row + 2 && i < Grid_size; i++)
    {
        for (int j = col; j < col + 2 && j < Grid_size; j++)
        {
            if (grid[i][j] == 'C' || grid[i][j] == 'D' || grid[i][j] == 'S' || grid[i][j] == 'B')
            {
                found = 1;
            }
        }
    }
    if (found)
    {
        printf("Enemy ships found in the area.\n");
    }
    else
    {
        printf("No enemy ships found.\n");
    }
}
void torpedo(char grid[10][10], char grid2[10][10], int target)
{
    if (target >= 1 && target <= Grid_size)
    {
        if (target < 0 || target > Grid_size)
        {
            printf("Invalid row. You lose your turn.\n");
            return;
        }
        for (int col = 0; col < Grid_size; col++)
        {
            fire(grid, grid2, target, col + 'A'); // Convert column number to letter ('A' to 'J')
        }
    }
    else
    {
        // If the target is not a valid row, check if it's a valid column (ASCII code for 'A' to 'J')
        int col = target - 'A';
        if (!(target >= 'A' && target <= 'J'))
        {
            printf("Invalid column. You lose your turn.\n");
            return;
        }
        for (int row = 1; row <= Grid_size; row++)
        {
            fire(grid, grid2, row, col + 'A'); // Fire at each row in the given column
        }
    }
}
void smokeScreen(char grid[10][10], int row, char column)
{
    int col;
    row--;
    col = toupper(column) - 'A';
    if (!validate(row, col))
    {
        return;
    }
    for (int i = row; i < row + 2 && i < Grid_size; i++)
    {
        for (int j = col; j < col + 2 && j < Grid_size; j++)
        {
            if (grid[i][j] == 'C' || grid[i][j] == 'D' || grid[i][j] == 'S' || grid[i][j] == 'B')
            {
                char c = grid[i][j];
                grid[i][j] = tolower(c);
            }
        }
    }
}
void artillery(char grid[10][10], char grid2[10][10], int row, char column)
{
    int col = toupper(column) - 'A';
    row--;

    if (!validate(row, col))
    {
        printf("Invalid coordinates for artillery strike.\n");
        return;
    }

    for (int i = row; i < row + 2 && i < Grid_size; i++)
    {
        for (int j = col; j < col + 2 && j < Grid_size; j++)
        {
            if (grid[i][j] == 'C' || grid[i][j] == 'D' || grid[i][j] == 'S' || grid[i][j] == 'B')
            {
                grid[i][j] = hit;
                grid2[i][j] = hit;
                printf("Hit at %c%d!\n", j + 'A', i + 1);
            }
            else
            {
                if (diff == 0)
                {
                    grid[i][j] = miss;
                    grid2[i][j] = miss;
                    printf("Miss at %c%d.\n", j + 'A', i + 1);
                }
            }
        }
    }
}
void clear_screen()
{
    system("cls");
}
