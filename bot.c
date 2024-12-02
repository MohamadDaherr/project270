#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include "function_headers.h"

#define MAX_QUEUE_SIZE 100

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
int diffBot;
int currentPlayer;
int smoke2 = 0;

char grid1[10][10], grid2[10][10];
char gridplayer1[10][10], gridplayer2[10][10];
char show[10][10];

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

char available_ships1[4] = {'C', 'B', 'D', 'S'};
char available_ships2[4] = {'C', 'B', 'D', 'S'};
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
            if (contains(available_ships2, 'B') == 1)
            {
                printf("Battleship ship sunk!\n");
                count++;
            }
        }
        else
        {
            if (contains(available_ships1, 'B') == 1)
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
            if (contains(available_ships2, 'D') == 1)
            {
                printf("Destroyer ship sunk!\n");
                count++;
            }
        }
        else
        {
            if (contains(available_ships1, 'D') == 1)
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
            if (contains(available_ships2, 'S') == 1)
            {
                printf("Submarine ship sunk!\n");
                count++;
            }
        }
        else
        {
            if (contains(available_ships1, 'S') == 1)
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

void print_Grid(char grid[10][10])
{
    printf("   A B C D E F G H I J\n");
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

void difficultyLevelBot()
{
    char difficulty[10];
    while (1)
    {
        printf("Choose difficulty Bot level (Easy - Medium - Hard): ");
        scanf("%s", difficulty);
        for (int i = 0; difficulty[i]; i++)
        {
            difficulty[i] = tolower(difficulty[i]);
        }
        if (strcmp(difficulty, "easy") == 0)
        {
            diffBot = 0;
            printf("You have chosen easy mode.\n");
            break;
        }
        else if (strcmp(difficulty, "medium") == 0)
        {
            diffBot = 1;
            printf("You have chosen medium mode.\n");
            break;
        }
        else if (strcmp(difficulty, "hard") == 0)
        {
            diffBot = 2;
            printf("You have chosen hard mode.\n");
            break;
        }
        else
        {
            printf("Invalid input. Please enter either 'easy' or 'hard'.\n");
        }
    }
}
void difficultyLevel()
{
    char difficulty[10];
    while (1)
    {
        printf("Choose difficulty level (Easy - Hard): ");
        scanf("%s", difficulty);
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
            printf("Invalid input. Please enter either 'easy' or 'hard'.\n");
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
                while (getchar() != '\n');
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
        printf("ERROR: Out of bounds at row %d, column %c\n", row + 1, col + 'A');
        return 0;
    }
    return 1;
}
int validateForQueue(int row, int col)
{
    if (col < 0 || col >= Grid_size || row < 0 || row >= Grid_size)
    {
        // printf("ERROR: Out of bounds at row %d, column %c\n", row + 1, col + 'A');
        return 0;
    }
    return 1;
}

void fire(char grid[10][10], char grid2[10][10], int row, char column)
{
    int col = toupper(column) - 'A';
    row--;
    if (validate(row, col))
    {
        if (toupper(grid[row][col]) == 'C' || toupper(grid[row][col]) == 'D' || toupper(grid[row][col]) == 'S' || toupper(grid[row][col]) == 'B' || grid[row][col] == '*')
        {
            grid[row][col] = hit;
            grid2[row][col] = hit;
            show[row][col] = hit;
        }
        else
        {
            if (diff == 0)
            {
                grid[row][col] = miss;
                grid2[row][col] = miss;
               print_Grid(gridplayer1);
            }
            print_Grid(show);
        }
    }
}

void radar_sweep(int row, char column, char grid[10][10])
{
    row--;
    int col = toupper(column) - 'A';
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
        // Target is a valid row
        for (int col = 0; col < Grid_size; col++)
        {
            fire(grid, grid2, target, col + 'A'); // Fire at each cell in the row
        }
    }
    else if (target >= 'A' && target <= 'J')
    {
        // Target is a valid column
        int col = target - 'A'; // Convert letter to column index
        for (int row = 1; row <= Grid_size; row++)
        {
            fire(grid, grid2, row, col + 'A'); // Fire at each cell in the column
        }
    }
    else
    {
        printf("Invalid torpedo target. You lose your turn.\n");
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
                if (diff == 0 && grid[i][j] == water)
                {
                    grid[i][j] = miss;
                    grid2[i][j] = miss;
                    printf("Miss at %c%d.\n", j + 'A', i + 1);
                }
            }
        }
    }
}

int radarUsed = 0;
int smokeScreensUsed = 0;
int artilleryAvailable = 0;
int torpedoAvailable = 0;
int missCount = 0;
int submarineHidden=0;
typedef struct
{
    int row;
    int col;
} Target;

typedef struct
{
    int row;         // Row of the first hit
    int col;         // Column of the first hit
    int orientation; // 0 = unknown, 1 = horizontal, 2 = vertical
    int processing;  // 1 = currently targeting this ship, 0 = no active target
} ShipTarget;
int countturnsbot = 0;

ShipTarget currentTarget = {-1, -1, 0, 0};

Target botTargetQueue[MAX_QUEUE_SIZE];
int front = -1, rear = -1;

Target horizontal[MAX_QUEUE_SIZE];
int front1 = -1, rear1 = -1;

Target vertical[MAX_QUEUE_SIZE];
int front2 = -1, rear2 = -1;

void enqueue(int row, int col)
{
    if (rear == MAX_QUEUE_SIZE - 1)
        return;
    if (front == -1)
        front = 0;
    botTargetQueue[++rear].row = row;
    botTargetQueue[rear].col = col;
}
void vertical_enqueue(int row, int col)
{
    if (rear2 == MAX_QUEUE_SIZE - 1)
        return;
    if (front2 == -1)
        front2 = 0;
    vertical[++rear2].row = row;
    vertical[rear2].col = col;
}
void horizontal_enqueue(int row, int col)
{
    if (rear1 == MAX_QUEUE_SIZE - 1)
        return;
    if (front1 == -1)
        front1 = 0;
    horizontal[++rear1].row = row;
    horizontal[rear1].col = col;
}
Target vertical_dequeue()
{
    Target t = {-1, -1};
    if (front2 == -1 || front2 > rear2)
        return t;
    t = vertical[front2++];
    return t;
}

Target horizontal_dequeue()
{
    Target t = {-1, -1};
    if (front1 == -1 || front1 > rear1)
        return t;
    t = horizontal[front1++];
    return t;
}

Target dequeue()
{
    Target t = {-1, -1};
    if (front == -1 || front > rear)
        return t;
    t = botTargetQueue[front++];
    return t;
}

int isQueueEmpty()
{
    return front == -1 || front > rear;
}
int isQueueEmptyhori()
{
    return front1 == -1 || front1 > rear1;
}
int isQueueEmptyvert()
{
    return front2 == -1 || front2 > rear2;
}

void enqueue_if_valid(int row, int column, char trackingGrid[10][10])
{
    if (validateForQueue(row, column) && trackingGrid[row][column] == water)
    {
        enqueue(row, column);
    }
}
void enqueue_if_valid_horizontal(int row, int col, char trackingGrid[10][10])
{
    if (validateForQueue(row, col) && trackingGrid[row][col] == water)
    {
        horizontal_enqueue(row, col);
    }
}

void enqueue_if_valid_vertical(int row, int col, char trackingGrid[10][10])
{
    if (validateForQueue(row, col) && trackingGrid[row][col] == water)
    {
        vertical_enqueue(row, col);
    }
}

int row_has_untargeted_cells(int row, char trackingGrid[10][10])
{
    for (int col = 0; col < Grid_size; col++)
    {
        if (trackingGrid[row - 1][col] == water)
        {             // Adjust for 1-indexed rows
            return 1; // Found untargeted cell
        }
    }
    return 0;
}

int column_has_untargeted_cells(char col, char trackingGrid[10][10])
{
    int columnIndex = toupper(col) - 'A';
    for (int row = 0; row < Grid_size; row++)
    {
        if (trackingGrid[row][columnIndex] == water)
        {
            return 1; // Found untargeted cell
        }
    }
    return 0;
}

void enqueue_adjacent_cells(int row, int col, char trackingGrid[10][10])
{
    if (validateForQueue(row - 1, col) && trackingGrid[row - 1][col] == water)
    {
        // printf("mhmd");
        enqueue(row - 1, col);
    }
    if (validateForQueue(row, col - 1) && trackingGrid[row][col - 1] == water)
    {
        // printf("ali");
        enqueue(row, col - 1);
    }
    if (validateForQueue(row + 1, col) && trackingGrid[row + 1][col] == water)
    {
        enqueue(row + 1, col);
    }
    if (validateForQueue(row, col + 1) && trackingGrid[row][col + 1] == water)
    {
        enqueue(row, col + 1);
    }
}
void artilleryBot(char grid[10][10], char grid2[10][10], int row, char column)
{
    int col = toupper(column) - 'A';
    row--;

    if (!validate(row, col))
    {
        printf("Invalid coordinates for artillery strike.\n");
        return;
    }

    int consecutiveHorizontal = 0;
    int consecutiveVertical = 0;

    for (int i = row; i < row + 2 && i < Grid_size; i++)
    {
        for (int j = col; j < col + 2 && j < Grid_size; j++)
        {
            if (toupper(grid[i][j]) == 'C' || toupper(grid[i][j]) == 'B' || toupper(grid[i][j]) == 'D' || toupper(grid[i][j]) == 'S')
            {
                grid[i][j] = hit;
                grid2[i][j] = hit;
                printf("Hit at %c%d!\n", j + 'A', i + 1);
                // Enqueue the current cell that contains a ship part
                    // enqueue(i, j);

                // Enqueue specific adjacent cells based on the position of the ship part
                    if (i == row && j == col) // Top-left cell
                    {
                        if (validateForQueue(i - 1, j) && grid2[i - 1][j] == water) // Above
                            enqueue_if_valid_vertical(i - 1, j,grid2);
                        if (validateForQueue(i, j - 1) && grid2[i][j - 1] == water) // Left
                            enqueue_if_valid_horizontal(i, j - 1,grid2);
                    }
                    else if (i == row && j == col + 1) // Top-right cell
                    {
                     
                        if (validateForQueue(i - 1, j) && grid2[i - 1][j] == water) // Above
                            enqueue_if_valid_vertical(i - 1, j,grid2);
                        if (validateForQueue(i, j + 1) && grid2[i][j + 1] == water) // Right
                            enqueue_if_valid_horizontal(i, j + 1,grid2);
                    }
                    else if (i == row + 1 && j == col) // Bottom-left cell
                    {
                        if (validateForQueue(i + 1, j) && grid2[i + 1][j] == water) // Below
                            enqueue_if_valid_vertical(i + 1, j,grid2);
                           
                        if (validateForQueue(i, j - 1) && grid2[i][j - 1] == water) // Left
                            enqueue_if_valid_horizontal(i, j - 1,grid2);
                    }
                    else if (i == row + 1 && j == col + 1) // Bottom-right cell
                    {
                        
                        if (validateForQueue(i + 1, j) && grid2[i + 1][j] == water) // Below
                            enqueue_if_valid_vertical(i + 1, j,grid2);
                        if (validateForQueue(i, j + 1) && grid2[i][j + 1] == water) // Right
                            enqueue_if_valid_horizontal(i, j + 1,grid2);
                    }
                // printf("ali");
            }
            else if ((diff == 0) && grid[i][j] == water)
            {
                grid[i][j] = miss;
                grid2[i][j] = miss;
                printf("Miss at %c%d.\n", j + 'A', i + 1);
            }
        }
    }

    // if(diff==1)
    // {
    //     for (int i = row; i < row + 2 && i < Grid_size; i++)
    //     {
    //         for (int j = col; j < col + 2 && j < Grid_size; j++)
    //         {
    //             if (grid2[i][j] == hit)
    //                 enqueue_adjacent_cells(i, j,grid2);
    //         }
    //     }
    // }


    // // Expand based on orientation
    // if (consecutiveHorizontal)
    // {
    //      printf("Detected horizontal orientation.\n");
    //     // Enqueue left and right cells in the same row
    //     for (int j = col; j < col + 2 && j < Grid_size; j++)
    //     {
    //         if (validateForQueue(row, j - 1) && grid2[row][j - 1] == water)
    //             horizontal_enqueue(row, j - 1);

    //         if (validateForQueue(row, j + 1) && grid2[row][j + 1] == water)
    //             horizontal_enqueue(row, j + 1);
    //     }

    // }
    // else if (consecutiveVertical)
    // {
    //      printf("Detected vertical orientation.\n");
    //     // Enqueue top and bottom cells in the same column
    //     for (int i = row; i < row + 2 && i < Grid_size; i++)
    //     {
    //         if (validateForQueue(i - 1, col) && grid2[i - 1][col] == water)
    //             vertical_enqueue(i - 1, col);

    //         if (validateForQueue(i + 1, col) && grid2[i + 1][col] == water)
    //             vertical_enqueue(i + 1, col);
    //     }

    // }
    // else
    // {
    //     printf("Diagonal or isolated hits detected; enqueuing in general queue.\n");
    //     for (int i = row; i < row + 2 && i < Grid_size; i++)
    //     {
    //         for (int j = col; j < col + 2 && j < Grid_size; j++)
    //         {
    //             if (grid2[i][j] == hit)
    //                 enqueue_adjacent_cells(i, j,grid2);
    //         }
    //     }
    // }
}

void artilleryBotEasy(char grid[10][10], char grid2[10][10], int row, char column)
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
            else if (diff == 0 && grid[i][j] == water)
            {
                grid[i][j] = miss;
                grid2[i][j] = miss;
                printf("Miss at %c%d.\n", j + 'A', i + 1);
            }
        }
    }

     for (int i = row; i < row + 2 && i < Grid_size; i++)
        {
            for (int j = col; j < col + 2 && j < Grid_size; j++)
            {
                if (grid2[i][j] == hit)
                    enqueue_adjacent_cells(i, j,grid2);
            }
        }
}
void torpedoBot(char grid[10][10], char grid2[10][10], int target)
{
    if (target >= 1 && target <= Grid_size)
    {
        // Target is a valid row
        int row = target - 1; // Convert to 0-based index
        for (int col = 0; col < Grid_size; col++)
        {
            if ((toupper(grid[row][col]) == 'C' || toupper(grid[row][col]) == 'D' ||
                 toupper(grid[row][col]) == 'S' || toupper(grid[row][col]) == 'B') &&
                grid2[row][col] != hit)
            {
                // Enqueue the hit for further processing
                // enqueue_adjacent_cells(row,col,grid2);
                enqueue_if_valid_vertical(row+1,col,grid2);
                enqueue_if_valid_vertical(row-1,col,grid2);

            }
            fire(grid, grid2, row + 1, col + 'A'); // Fire at each cell in the row
        }
    }
    else if (target >= 'A' && target <= 'J')
    {
        // Target is a valid column
        int col = target - 'A'; // Convert letter to column index
        for (int row = 0; row < Grid_size; row++)
        {
            if ((toupper(grid[row][col]) == 'C' || toupper(grid[row][col]) == 'D' ||
                 toupper(grid[row][col]) == 'S' || toupper(grid[row][col]) == 'B') &&
                grid2[row][col] != hit)
            {
                // Enqueue the hit for further processing
                enqueue_if_valid_horizontal(row,col-1,grid2);
                enqueue_if_valid_horizontal(row,col+1,grid2);
            }
            fire(grid, grid2, row + 1, col + 'A'); // Fire at each cell in the column
        }
    }
    else
    {
        printf("Invalid torpedo target. The bot loses its turn.\n");
    }
}
void radar_sweep_and_process(int row, char column, char opponentGrid[10][10], char trackingGrid[10][10])
{
    printf("Bot is scanning a 2x2 area with Radar at %c%d...\n", column, row);

    row--;                // Convert to 0-based index
    column = column - 'A'; // Convert column to 0-based index
    int found = 0;
    int allWater=0;
         // Flag to check if all cells are water

    // Loop through the 2x2 area
    for (int i = row; i < row + 2 && i < Grid_size; i++)
    {
        for (int j = column; j < column + 2 && j < Grid_size; j++)
        {
            if (validateForQueue(i, j)) 
            {
                // Check if the current cell contains a ship (C, B, D, S)
                if (opponentGrid[i][j] == 'C' || opponentGrid[i][j] == 'B' || opponentGrid[i][j]== 'D' || opponentGrid[i][j] == 'S')
                {
                     // At least one cell contains a ship, so it's not all water
                    found = 1;     // A ship part is detected
                    printf("Ship detected at %c%d\n", j + 'A', i + 1);

                    // Enqueue the current cell that contains a ship part
                    enqueue(i, j);

                    // Enqueue specific adjacent cells based on the position of the ship part
                    if (i == row && j == column) // Top-left cell
                    {
                        // Enqueue top-left cell and adjacent left & above if valid
                        if (validateForQueue(i - 1, j) && trackingGrid[i - 1][j] == water) // Above
                            enqueue(i - 1, j);
                        if (validateForQueue(i, j - 1) && trackingGrid[i][j - 1] == water) // Left
                            enqueue(i, j - 1);
                    }
                    else if (i == row && j == column + 1) // Top-right cell
                    {
                        // Enqueue top-right cell and adjacent right & above if valid
                        if (validateForQueue(i - 1, j) && trackingGrid[i - 1][j] == water) // Above
                            enqueue(i - 1, j);
                        if (validateForQueue(i, j + 1) && trackingGrid[i][j + 1] == water) // Right
                            enqueue(i, j + 1);
                    }
                    else if (i == row + 1 && j == column) // Bottom-left cell
                    {
                        // Enqueue bottom-left cell and adjacent left & below if valid
                        if (validateForQueue(i + 1, j) && trackingGrid[i + 1][j] == water) // Below
                            enqueue(i + 1, j);
                        if (validateForQueue(i, j - 1) && trackingGrid[i][j - 1] == water) // Left
                            enqueue(i, j - 1);
                    }
                    else if (i == row + 1 && j == column + 1) // Bottom-right cell
                    {
                        // Enqueue bottom-right cell and adjacent right & below if valid
                        if (validateForQueue(i + 1, j) && trackingGrid[i + 1][j] == water) // Below
                            enqueue(i + 1, j);
                        if (validateForQueue(i, j + 1) && trackingGrid[i][j + 1] == water) // Right
                            enqueue(i, j + 1);
                    }
                }
                else if (opponentGrid[i][j] == water)
                {
                    // If the cell is water, enqueue it anyway (unless all cells are water)
                        allWater++; // Enqueue the water cell if any ship was found
                        enqueue(i, j);
                }
            }
        }
    }

    // If all cells are water, skip enqueuing any cells
    if (allWater==4)
    {
        for(int i=0; i<4;i++){
            dequeue();
        }printf("All cells in the 2x2 area are water. Skipping enqueuing.\n");
        return; // Exit the function if no ships were found
    }

    if (found)
    {
        printf("Ships detected: Enqueuing specific adjacent cells.\n");
    }
    else
    {
        printf("No enemy ships found in the scanned area.\n");
    }
    // printf("Bot is scanning a 2x2 area with Radar at %c%d...\n", column, row);

    // row--;                // Convert to 0-based index
    // column = column - 'A'; // Convert column to 0-based index
    // int found = 0;
    // int allWater = 0;     // Flag to check if all cells are water

    // // Loop through the 2x2 area
    // for (int i = row; i < row + 2 && i < Grid_size; i++)
    // {
    //     for (int j = column; j < column + 2 && j < Grid_size; j++)
    //     {
    //         if (validateForQueue(i, j)) 
    //         {
    //             // Check if the current cell contains a ship (C, B, D, S)
    //             if (opponentGrid[i][j] == 'C' || opponentGrid[i][j] == 'B' || opponentGrid[i][j]== 'D' || opponentGrid[i][j] == 'S')
    //             {
    //                 found = 1;     // A ship part is detected
    //                 printf("Ship detected at %c%d\n", j + 'A', i + 1);

    //                 // Enqueue the current cell that contains a ship part
    //                 enqueue(i, j);

    //                 // Enqueue specific adjacent cells based on the position of the ship part
    //                 if (i == row && j == column) // Top-left cell
    //                 {
    //                     if (validateForQueue(i - 1, j) && trackingGrid[i - 1][j] == water) // Above
    //                         enqueue(i - 1, j);
    //                     if (validateForQueue(i, j - 1) && trackingGrid[i][j - 1] == water) // Left
    //                         enqueue(i, j - 1);
    //                 }
    //                 else if (i == row && j == column + 1) // Top-right cell
    //                 {
    //                     if (validateForQueue(i - 1, j) && trackingGrid[i - 1][j] == water) // Above
    //                         enqueue(i - 1, j);
    //                     if (validateForQueue(i, j + 1) && trackingGrid[i][j + 1] == water) // Right
    //                         enqueue(i, j + 1);
    //                 }
    //                 else if (i == row + 1 && j == column) // Bottom-left cell
    //                 {
    //                     if (validateForQueue(i + 1, j) && trackingGrid[i + 1][j] == water) // Below
    //                         enqueue(i + 1, j);
    //                     if (validateForQueue(i, j - 1) && trackingGrid[i][j - 1] == water) // Left
    //                         enqueue(i, j - 1);
    //                 }
    //                 else if (i == row + 1 && j == column + 1) // Bottom-right cell
    //                 {
    //                     if (validateForQueue(i + 1, j) && trackingGrid[i + 1][j] == water) // Below
    //                         enqueue(i + 1, j);
    //                     if (validateForQueue(i, j + 1) && trackingGrid[i][j + 1] == water) // Right
    //                         enqueue(i, j + 1);
    //                 }
    //             }
    //             else if (opponentGrid[i][j] == water)
    //             {
    //                 // If the cell is water, enqueue it anyway (unless all cells are water)
    //                     allWater++;                   
    //                     enqueue(i, j); // Enqueue the water cell if any ship was found
    //             }
    //         }
    //     }
    // }


    // // If all cells are water, skip enqueuing any cells
    // if (allWater==4)
    // {
    //     for (int i = 0; i < 4; i++)
    //     {
    //         dequeue();
    //     }
        
    //     printf("All cells in the 2x2 area are water. Skipping enqueuing.\n");
    //     return; // Exit the function if no ships were found
    // }

    // if (found)
    // {
    //     printf("Ships detected: Enqueuing specific adjacent cells.\n");
    // }
    // else
    // {
    //     printf("No enemy ships found in the scanned area.\n");
    // }
}



void bot_hard_play(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10])
{

    printf("Bot is taking its turn...\n");

    // Step 2: Use torpedo if available
   if (torpedoAvailable)
{
    printf("Bot using Torpedo...\n");

    int maxRowWater = -1;   // Maximum water cells in a row
    int maxColWater = -1;   // Maximum water cells in a column
    int targetRow = -1;     // Row with maximum water cells
    int targetCol = -1;     // Column with maximum water cells

    // Calculate maximum water cells in each row
    for (int row = 1; row <= Grid_size; row++) // Rows are 1-indexed
    {
        int waterCount = 0;
        for (int col = 0; col < Grid_size; col++)
        {
            if (trackingGrid[row - 1][col] == water) // Check for water cells in the row
                waterCount++;
        }
        if (waterCount > maxRowWater) // Update max row water count
        {
            maxRowWater = waterCount;
            targetRow = row;
        }
    }

    // Calculate maximum water cells in each column
    for (int col = 0; col < Grid_size; col++) // Columns are 0-indexed
    {
        int waterCount = 0;
        for (int row = 0; row < Grid_size; row++)
        {
            if (trackingGrid[row][col] == water) // Check for water cells in the column
                waterCount++;
        }
        if (waterCount > maxColWater) // Update max column water count
        {
            maxColWater = waterCount;
            targetCol = col;
        }
    }

    // Compare maxRowWater and maxColWater to determine the target
    if (maxRowWater >= maxColWater)
    {
        printf("Bot targeting row %d with the most water cells (%d cells).\n", targetRow, maxRowWater);
        torpedoBot(opponentGrid, trackingGrid, targetRow); // Fire torpedo at the row
    }
    else
    {
        printf("Bot targeting column %c with the most water cells (%d cells).\n", targetCol + 'A', maxColWater);
        torpedoBot(opponentGrid, trackingGrid, targetCol + 'A'); // Fire torpedo at the column
    }

    print_Grid(trackingGrid); // Display the tracking grid after firing
    torpedoAvailable = 0;     // Torpedo is one-time use
    return;                   // End the turn after using torpedo
}

    if (artilleryAvailable)
    {
        printf("Bot using Artillery...\n");
        int row, col;
        do
        {
            row = rand() % (Grid_size - 1);
            col = rand() % (Grid_size - 1);
        } while (trackingGrid[row][col] != water); // Ensure the cell is untargeted

        artilleryBot(opponentGrid, trackingGrid, row + 1, col + 'A');
        print_Grid(trackingGrid);
        artilleryAvailable = 0; // Artillery is one-time use
        return;                 // End the turn after using artillery
    }


    // smoke
    if (smoke2 > 1 && artilleryAvailable == 0 && isQueueEmpty() == 1&& isQueueEmptyhori() == 1 && isQueueEmptyvert() == 1)
{
    printf("Bot is deciding where to use the Smoke Screen...\n");

    // Step 3.1: Prioritize hiding the submarine ('S') only once
    int found = 0;
    if (!submarineHidden) // Only proceed if the submarine hasn't been hidden yet
    {
        for (int row = 0; row < Grid_size && !found; row++)
        {
            for (int col = 0; col < Grid_size; col++)
            {
                if (toupper(botGrid[row][col]) == 'S') // Check for submarine
                {
                    printf("Bot using Smoke to hide Submarine at %c%d...\n", col + 'A', row + 1);
                    smokeScreen(botGrid, row + 1, col + 'A'); // Apply smokescreen
                    smoke2--; // Decrement available smokescreens
                    submarineHidden = 1; // Mark submarine as hidden
                    found = 1;
                    return; // End turn after using smokescreen
                }
            }
        }
    }

    // Step 3.2: If no submarine is found or it has already been hidden, hide other ships
    if (!found)
    {
        for (int row = 0; row < Grid_size && !found; row++)
        {
            for (int col = 0; col < Grid_size; col++)
            {
                if (botGrid[row][col] == 'C' || botGrid[row][col] == 'B' || botGrid[row][col] == 'D') // Check for other ships
                {
                    printf("Bot using Smoke to hide ship at %c%d...\n", col + 'A', row + 1);
                    smokeScreen(botGrid, row + 1, col + 'A'); // Apply smokescreen
                    smoke2--; // Decrement available smokescreens
                    found = 1;
                    return; // End turn after using smokescreen
                }
            }
        }
    }
}

    // Step 3: Use radar sweep after 7 misses
    if (countturnsbot >= 6 && radarUsed < 3 && isQueueEmpty() == 1 && isQueueEmptyhori() == 1 && isQueueEmptyvert() == 1)
{
     printf("Bot deciding whether to use Radar Sweep...\n");

    int row = -1, col = -1;
    int validArea = 0;

    // Seed the random number generator
    srand(time(NULL));

    // Try up to 10 random attempts to find a valid 2x2 area of water
    for (int attempts = 0; attempts < 10 && !validArea; attempts++) {
        // Generate random row and column indices for the top-left corner of the 2x2 area
        row = rand() % (Grid_size - 1);  // Ensure there is space for a 2x2 block
        col = rand() % (Grid_size - 1);  // Ensure there is space for a 2x2 block

        // Check if the 2x2 block formed by the top-left corner is all water
        if (trackingGrid[row][col] == water && trackingGrid[row][col + 1] == water &&
            trackingGrid[row + 1][col] == water && trackingGrid[row + 1][col + 1] == water) {
            validArea = 1;  // Mark as valid if all cells in the 2x2 area are water
        }
    }

    if (validArea) {
        // Found a valid 2x2 water area
        printf("Radar Sweep targeting 2x2 water area starting at %c%d...\n", col + 'A', row + 1);

        // Call radar sweep process (modify as per your radar logic)
        radar_sweep_and_process(row + 1, col + 'A', opponentGrid, trackingGrid);

        radarUsed++;
        countturnsbot = 0; // Reset miss count
        return; // End turn after using radar
    } else {
        // No valid 2x2 water area found after 10 attempts
        printf("No valid 2x2 water area found for Radar Sweep.\n");
    }

}
    // while (!isQueueEmpty() || !isQueueEmptyhori() || !isQueueEmptyvert())
    // {

    //     // Step 1: Process Horizontal Queue
    //     if (!isQueueEmptyhori())
    //     {
    //         Target hor = horizontal_dequeue();
    //         while (!validate(hor.row, hor.col) || trackingGrid[hor.row][hor.col] != water)
    //         {
    //             if (isQueueEmptyhori())
    //                 return; // No valid targets in horizontal queue
    //             hor = horizontal_dequeue();
    //         }

    //         printf("Bot firing at %c%d from horizontal queue...\n", hor.col + 'A', hor.row + 1);
    //         fire(opponentGrid, trackingGrid, hor.row + 1, hor.col + 'A');

    //         if (trackingGrid[hor.row][hor.col] == hit)
    //         {
    //             enqueue_if_valid_horizontal(hor.row, hor.col - 1, trackingGrid); // Left
    //             enqueue_if_valid_horizontal(hor.row, hor.col + 1, trackingGrid); // Right
    //         }
    //         return;
    //     }

    //     // Step 2: Process Vertical Queue
    //     if (!isQueueEmptyvert())
    //     {
    //         Target ver = vertical_dequeue();
    //         while (!validate(ver.row, ver.col) || trackingGrid[ver.row][ver.col] != water)
    //         {
    //             if (isQueueEmptyvert())
    //                 return; // No valid targets in vertical queue
    //             ver = vertical_dequeue();
    //         }

    //         printf("Bot firing at %c%d from vertical queue...\n", ver.col + 'A', ver.row + 1);
    //         fire(opponentGrid, trackingGrid, ver.row + 1, ver.col + 'A');

    //         if (trackingGrid[ver.row][ver.col] == hit)
    //         {
    //             enqueue_if_valid_vertical(ver.row - 1, ver.col, trackingGrid); // Up
    //             enqueue_if_valid_vertical(ver.row + 1, ver.col, trackingGrid); // Down
    //         }
    //         return;
    //     }

    //     // Step 3: Process General Queue
    //     if (!isQueueEmpty())
    //     {
    //         Target t = dequeue();
    //         while (!validate(t.row, t.col) || trackingGrid[t.row][t.col] != water)
    //         {
    //             if (isQueueEmpty())
    //                 return; // No valid targets in general queue
    //             t = dequeue();
    //         }

    //         printf("Bot firing at %c%d from general queue...\n", t.col + 'A', t.row + 1);
    //         fire(opponentGrid, trackingGrid, t.row + 1, t.col + 'A');

    //         if (trackingGrid[t.row][t.col] == hit)
    //         {
    //             if (currentTarget.processing == 0)
    //             {
    //                 // First hit, initialize targeting
    //                 currentTarget.row = t.row;
    //                 currentTarget.col = t.col;
    //                 currentTarget.processing = 1;
    //             }
    //             else if (currentTarget.orientation == 0)
    //             {
    //                 // Determine orientation on second hit
    //                 if (t.row == currentTarget.row)
    //                 {
    //                     currentTarget.orientation = 1; // Horizontal
    //                     printf("\nHORIZONTAL ORIENTATION\n");
    //                     if(t.col+1==currentTarget.col){
    //                         enqueue_if_valid_horizontal(t.row, t.col - 1, trackingGrid); // Left
    //                         enqueue_if_valid_horizontal(t.row, currentTarget.col + 1, trackingGrid); // Right
    //                     }
    //                     else if(t.col-1==currentTarget.col){
    //                         enqueue_if_valid_horizontal(t.row, t.col + 1, trackingGrid); // Right
    //                         enqueue_if_valid_horizontal(t.row, currentTarget.col - 1, trackingGrid); // Left
    //                     }
    //                     // enqueue_if_valid_horizontal(t.row, currentTarget.col - 1, trackingGrid); // Left
    //                     // enqueue_if_valid_horizontal(t.row, currentTarget.col + 1, trackingGrid); // Right
    //                 }
    //                 else if (t.col == currentTarget.col)
    //                 {
    //                     currentTarget.orientation = 2; // Vertical
    //                     printf("\nVERTICAL ORIENTATION\n");
    //                     if(t.col+1==currentTarget.col){
    //                         enqueue_if_valid_vertical(t.row - 1, t.col, trackingGrid); // Up
    //                     enqueue_if_valid_vertical(currentTarget.row + 1, t.col, trackingGrid); // Down
    //                     }
    //                     else if(t.col-1==currentTarget.col){
    //                         enqueue_if_valid_vertical(t.row + 1, t.col, trackingGrid); // Down
    //                         enqueue_if_valid_vertical(currentTarget.row - 1, t.col, trackingGrid); // Up
    //                     }
    //                     // enqueue_if_valid_vertical(currentTarget.row - 1, t.col, trackingGrid); // Up
    //                     // enqueue_if_valid_vertical(currentTarget.row + 1, t.col, trackingGrid); // Down
    //                 }
    //             }
    //             else
    //             {
    //                 // Already have orientation; enqueue accordingly
    //                 if (currentTarget.orientation == 1) // Horizontal
    //                 {
    //                     enqueue_if_valid_horizontal(t.row, t.col - 1, trackingGrid); // Left
    //                     enqueue_if_valid_horizontal(t.row, t.col + 1, trackingGrid); // Right
    //                 }
    //                 else if (currentTarget.orientation == 2) // Vertical
    //                 {
    //                     enqueue_if_valid_vertical(t.row - 1, t.col, trackingGrid); // Up
    //                     enqueue_if_valid_vertical(t.row + 1, t.col, trackingGrid); // Down
    //                 }
    //             }
    //         }
    //         return;
    //     }
    // }
     while (!isQueueEmpty() || !isQueueEmptyhori() || !isQueueEmptyvert())
    {

        // Step 1: Process Horizontal Queue
        if (!isQueueEmptyhori())
        {
            Target hor = horizontal_dequeue();
            while (!validate(hor.row, hor.col) || trackingGrid[hor.row][hor.col] != water)
            {
                if (isQueueEmptyhori())
                    return; // No valid targets in horizontal queue
                hor = horizontal_dequeue();
            }

            printf("Bot firing at %c%d from horizontal queue...\n", hor.col + 'A', hor.row + 1);
            fire(opponentGrid, trackingGrid, hor.row + 1, hor.col + 'A');

            if (trackingGrid[hor.row][hor.col] == hit)
            {
                enqueue_if_valid_horizontal(hor.row, hor.col - 1, trackingGrid); // Left
                enqueue_if_valid_horizontal(hor.row, hor.col + 1, trackingGrid); // Right
            }
            return;
        }

        // Step 2: Process Vertical Queue
        if (!isQueueEmptyvert())
        {
            Target ver = vertical_dequeue();
            while (!validate(ver.row, ver.col) || trackingGrid[ver.row][ver.col] != water)
            {
                if (isQueueEmptyvert())
                    return; // No valid targets in vertical queue
                ver = vertical_dequeue();
            }

            printf("Bot firing at %c%d from vertical queue...\n", ver.col + 'A', ver.row + 1);
            fire(opponentGrid, trackingGrid, ver.row + 1, ver.col + 'A');

            if (trackingGrid[ver.row][ver.col] == hit)
            {
                enqueue_if_valid_vertical(ver.row - 1, ver.col, trackingGrid); // Up
                enqueue_if_valid_vertical(ver.row + 1, ver.col, trackingGrid); // Down
            }
            return;
        }

        // Step 3: Process General Queue
        if (!isQueueEmpty())
        {
            Target t = dequeue();
            while (!validate(t.row, t.col) || trackingGrid[t.row][t.col] != water)
            {
                if (isQueueEmpty())
                    return; // No valid targets in general queue
                t = dequeue();
            }

            printf("Bot firing at %c%d from general queue...\n", t.col + 'A', t.row + 1);
            fire(opponentGrid, trackingGrid, t.row + 1, t.col + 'A');

            if (trackingGrid[t.row][t.col] == hit)
            {
                if (currentTarget.processing == 0)
                {
                    // First hit, initialize targeting
                    currentTarget.row = t.row;
                    currentTarget.col = t.col;
                    currentTarget.processing = 1;
                }
                else if (currentTarget.orientation == 0)
                {
                    // Determine orientation on second hit
                    if (t.row == currentTarget.row)
                    {
                        currentTarget.orientation = 1; // Horizontal
                        printf("\nHORIZONTAL ORIENTATION\n");
                        enqueue_if_valid_horizontal(t.row, t.col - 1, trackingGrid); // Left
                        enqueue_if_valid_horizontal(t.row, t.col + 1, trackingGrid); // Right
                    }
                    else if (t.col == currentTarget.col)
                    {
                        currentTarget.orientation = 2; // Vertical
                        printf("\nVERTICAL ORIENTATION\n");
                        enqueue_if_valid_vertical(t.row - 1, t.col, trackingGrid); // Up
                        enqueue_if_valid_vertical(t.row + 1, t.col, trackingGrid); // Down
                    }
                }
                else
                {
                    // Already have orientation; enqueue accordingly
                    if (currentTarget.orientation == 1) // Horizontal
                    {
                        enqueue_if_valid_horizontal(t.row, t.col - 1, trackingGrid); // Left
                        enqueue_if_valid_horizontal(t.row, t.col + 1, trackingGrid); // Right
                    }
                    else if (currentTarget.orientation == 2) // Vertical
                    {
                        enqueue_if_valid_vertical(t.row - 1, t.col, trackingGrid); // Up
                        enqueue_if_valid_vertical(t.row + 1, t.col, trackingGrid); // Down
                    }
                }
            }
            return;
        }
    }
    // Step 4: Random Targeting
    bot_fire_random_until_hit(opponentGrid, trackingGrid);
}


void bot_fire_random_until_hit(char opponentGrid[10][10], char trackingGrid[10][10])
{
    currentTarget.processing = 0;
    currentTarget.orientation = 0;

    int row, col;
    int valid = 0;

    while (!valid)
    {
        row = rand() % Grid_size;
        col = rand() % Grid_size;

        // Check if the selected cell is untargeted and not radar-swept
        if (trackingGrid[row][col] == water)
        {
            valid = 1;

            // Check surrounding cells to ensure no neighbors are hit
            if (trackingGrid[row - 1][col] != water)
                valid = 0; // Up
            if (trackingGrid[row + 1][col] != water)
                valid = 0; // Down
            if (trackingGrid[row][col - 1] != water)
                valid = 0; // Left
            if (trackingGrid[row][col + 1] != water)
                valid = 0; // Right
            if (opponentGrid[row][col] == 'r')
                valid = 0; // Skip cells marked as radar-swept
        }
    }

    // Fire at the valid cell
    printf("Bot firing at %c%d...\n", col + 'A', row + 1);
    fire(opponentGrid, trackingGrid, row + 1, col + 'A');

    if (trackingGrid[row][col] == hit)
    {
        // First hit: initialize targeting and add adjacent cells
        currentTarget.row = row;
        currentTarget.col = col;
        currentTarget.processing = 1;
        enqueue_adjacent_cells(row, col, trackingGrid);
    }
}

void bot_fire_random_easy_medium(char opponentGrid[10][10], char trackingGrid[10][10])
{
    currentTarget.processing = 0;
    currentTarget.orientation = 0;

    int row, col;
    int valid = 0;

    while (!valid)
    {
        row = rand() % Grid_size;
        col = rand() % Grid_size;

        // Check if the selected cell is untargeted and not radar-swept
        if (trackingGrid[row][col] == water)
        {
            valid = 1;
        }
    }

    // Fire at the valid cell
    printf("Bot firing at %c%d...\n", col + 'A', row + 1);
    fire(opponentGrid, trackingGrid, row + 1, col + 'A');

    if (trackingGrid[row][col] == hit)
    {
        // First hit: initialize targeting and add adjacent cells
        currentTarget.row = row;
        currentTarget.col = col;
        currentTarget.processing = 1;
        enqueue_adjacent_cells(row, col, trackingGrid);
    }
}

void place_ships_randomly(char grid[10][10])
{
    srand(time(NULL));
    for (int i = 0; i < total_ships; i++)
    {
        int placed = 0;
        while (!placed)
        {
            int row = rand() % Grid_size;
            int col = rand() % Grid_size;
            char orientation = (rand() % 2) ? 'H' : 'V';
            if (check_coordinates(row, col, ships[i].size, orientation, grid))
            {
                for (int j = 0; j < ships[i].size; j++)
                {
                    if (orientation == 'H')
                    {
                        grid[row][col + j] = ships[i].name[0];
                    }
                    else
                    {
                        grid[row + j][col] = ships[i].name[0];
                    }
                }
                placed = 1;
            }
        }
    }
}
void clear_screen()
{
    system("cls");
}




void bot_easy_play(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10])
{

    

    // Step 2: Use Torpedo if available and queue is empty
    if (torpedoAvailable)
    {
        printf("Easy Bot using Torpedo...\n");

        // Target a row or column with the most untargeted cells
        int maxRowWater = -1, maxColWater = -1, targetRow = -1, targetCol = -1;

        // Check rows for untargeted cells
        for (int row = 0; row < Grid_size; row++)
        {
            int waterCount = 0;
            for (int col = 0; col < Grid_size; col++)
            {
                if (trackingGrid[row][col] == water)
                    waterCount++;
            }
            if (waterCount > maxRowWater)
            {
                maxRowWater = waterCount;
                targetRow = row + 1; // 1-indexed
            }
        }

        // Check columns for untargeted cells
        for (int col = 0; col < Grid_size; col++)
        {
            int waterCount = 0;
            for (int row = 0; row < Grid_size; row++)
            {
                if (trackingGrid[row][col] == water)
                    waterCount++;
            }
            if (waterCount > maxColWater)
            {
                maxColWater = waterCount;
                targetCol = col + 'A'; // Column as letter
            }
        }

        // Fire at the row or column with the most untargeted cells
        if (maxRowWater >= maxColWater)
        {
            printf("Targeting row %d with Torpedo...\n", targetRow);
            torpedoBot(opponentGrid, trackingGrid, targetRow);
        }
        else
        {
            printf("Targeting column %c with Torpedo...\n", targetCol);
            torpedoBot(opponentGrid, trackingGrid, targetCol);
        }

        torpedoAvailable = 0; // Torpedo is one-time use
        return; // End turn after using Torpedo
    }

    if (artilleryAvailable)
    {
        printf("Bot using Artillery...\n");
        int row, col;
        do
        {
            row = rand() % (Grid_size - 1);
            col = rand() % (Grid_size - 1);
        } while (trackingGrid[row][col] != water); // Ensure the cell is untargeted

        artilleryBotEasy(opponentGrid, trackingGrid, row + 1, col + 'A');
        print_Grid(trackingGrid);
        artilleryAvailable = 0; // Artillery is one-time use
        return;                 // End the turn after using artillery
    }

    // Step 3: Use Radar after 5 moves if it hasn’t been used yet
    // Step 3: Use radar sweep after 7 misses
    if (countturnsbot >= 6 && radarUsed < 3 && isQueueEmpty() == 1)
{
     printf("Bot deciding whether to use Radar Sweep...\n");

    int row = -1, col = -1;
    int validArea = 0;

    // Seed the random number generator
    srand(time(NULL));

    // Try up to 10 random attempts to find a valid 2x2 area of water
    for (int attempts = 0; attempts < 10 && !validArea; attempts++) {
        // Generate random row and column indices for the top-left corner of the 2x2 area
        row = rand() % (Grid_size - 1);  // Ensure there is space for a 2x2 block
        col = rand() % (Grid_size - 1);  // Ensure there is space for a 2x2 block

        // Check if the 2x2 block formed by the top-left corner is all water
        if (trackingGrid[row][col] == water && trackingGrid[row][col + 1] == water &&
            trackingGrid[row + 1][col] == water && trackingGrid[row + 1][col + 1] == water) {
            validArea = 1;  // Mark as valid if all cells in the 2x2 area are water
        }
    }

    if (validArea) {
        // Found a valid 2x2 water area
        printf("Radar Sweep targeting 2x2 water area starting at %c%d...\n", col + 'A', row + 1);

        // Call radar sweep process (modify as per your radar logic)
        radar_sweep_and_process(row + 1, col + 'A', opponentGrid, trackingGrid);
        radarUsed++;
        countturnsbot = 0; // Reset miss count
        return; // End turn after using radar
    } else {
        // No valid 2x2 water area found after 10 attempts
        printf("No valid 2x2 water area found for Radar Sweep.\n");
    }

}

    // Step 4: Use Smoke if available
    if (smoke2 > 1 && artilleryAvailable == 0 && isQueueEmpty() == 1&& isQueueEmptyhori() == 1 && isQueueEmptyvert() == 1)
{
    printf("Bot is deciding where to use the Smoke Screen...\n");

    // Step 3.1: Look for any random ship and hide it (no specific prioritization)
    int found = 0;
    for (int row = 0; row < Grid_size && !found; row++)
    {
        for (int col = 0; col < Grid_size; col++)
        {
            // Check for any ship (Carrier, Battleship, Destroyer, or Submarine)
            if (botGrid[row][col] == 'C' || botGrid[row][col] == 'B' || botGrid[row][col] == 'D' || botGrid[row][col] == 'S')
            {
                printf("Bot using Smoke to hide ship at %c%d...\n", col + 'A', row + 1);
                smokeScreen(botGrid, row + 1, col + 'A'); // Apply smokescreen
                smoke2--; // Decrement available smokescreens
                found = 1; // Mark that a ship has been hidden
                return; // End the turn after using smokescreen
            }
        }
    }

    // Step 3.2: If no ships are found, skip using the smokescreen
    if (!found)
    {
        printf("No ships available to hide with Smoke.\n");
    }
}

    // Step 1: Target cells in the queue
    while (!isQueueEmpty())
    {
        Target t = dequeue();
        printf("Medium Bot targeting queued cell at %c%d...\n", t.col + 'A', t.row + 1);

        // Check if the cell is already hit or missed
        if (trackingGrid[t.row][t.col] == hit || trackingGrid[t.row][t.col] == miss)
        {
            continue; // Skip already hit or missed cells
        }

            fire(opponentGrid, trackingGrid, t.row + 1, t.col + 'A');
            if (trackingGrid[t.row][t.col] == hit)
            {
                enqueue_adjacent_cells(t.row, t.col, trackingGrid);
            }
        return; // End turn after using the queue
    }

    // Step 5: Fire randomly if no other moves are available
    printf("Medium Bot firing randomly...\n");
    bot_fire_random_easy_medium(opponentGrid, trackingGrid);
    // Increment move count for Radar timing
    // moveCount++; 
}

void bot_medium_play(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10])
{
     printf("Bot is taking its turn...\n");

    // Step 2: Use torpedo if available
   if (torpedoAvailable)
{
    printf("Bot using Torpedo...\n");

    int maxRowWater = -1;   // Maximum water cells in a row
    int maxColWater = -1;   // Maximum water cells in a column
    int targetRow = -1;     // Row with maximum water cells
    int targetCol = -1;     // Column with maximum water cells

    // Calculate maximum water cells in each row
    for (int row = 1; row <= Grid_size; row++) // Rows are 1-indexed
    {
        int waterCount = 0;
        for (int col = 0; col < Grid_size; col++)
        {
            if (trackingGrid[row - 1][col] == water) // Check for water cells in the row
                waterCount++;
        }
        if (waterCount > maxRowWater) // Update max row water count
        {
            maxRowWater = waterCount;
            targetRow = row;
        }
    }

    // Calculate maximum water cells in each column
    for (int col = 0; col < Grid_size; col++) // Columns are 0-indexed
    {
        int waterCount = 0;
        for (int row = 0; row < Grid_size; row++)
        {
            if (trackingGrid[row][col] == water) // Check for water cells in the column
                waterCount++;
        }
        if (waterCount > maxColWater) // Update max column water count
        {
            maxColWater = waterCount;
            targetCol = col;
        }
    }

    // Compare maxRowWater and maxColWater to determine the target
    if (maxRowWater >= maxColWater)
    {
        printf("Bot targeting row %d with the most water cells (%d cells).\n", targetRow, maxRowWater);
        torpedoBot(opponentGrid, trackingGrid, targetRow); // Fire torpedo at the row
    }
    else
    {
        printf("Bot targeting column %c with the most water cells (%d cells).\n", targetCol + 'A', maxColWater);
        torpedoBot(opponentGrid, trackingGrid, targetCol + 'A'); // Fire torpedo at the column
    }

    print_Grid(trackingGrid); // Display the tracking grid after firing
    torpedoAvailable = 0;     // Torpedo is one-time use
    return;                   // End the turn after using torpedo
}

    if (artilleryAvailable)
    {
        printf("Bot using Artillery...\n");
        int row, col;
        do
        {
            row = rand() % (Grid_size - 1);
            col = rand() % (Grid_size - 1);
        } while (trackingGrid[row][col] != water); // Ensure the cell is untargeted

        artilleryBot(opponentGrid, trackingGrid, row + 1, col + 'A');
        print_Grid(trackingGrid);
        artilleryAvailable = 0; // Artillery is one-time use
        return;                 // End the turn after using artillery
    }

    if (smoke2 > 1 && artilleryAvailable == 0 && isQueueEmpty() == 1 && isQueueEmptyhori() == 1 && isQueueEmptyvert() == 1)
{
    printf("Bot is deciding where to use the Smoke Screen...\n");

    // Step 3.1: Look for any random ship and hide it (no specific prioritization)
    int found = 0;
    for (int row = 0; row < Grid_size && !found; row++)
    {
        for (int col = 0; col < Grid_size; col++)
        {
            // Check for any ship (Carrier, Battleship, Destroyer, or Submarine)
            if (botGrid[row][col] == 'C' || botGrid[row][col] == 'B' || botGrid[row][col] == 'D' || botGrid[row][col] == 'S')
            {
                printf("Bot using Smoke to hide ship at %c%d...\n", col + 'A', row + 1);
                smokeScreen(botGrid, row + 1, col + 'A'); // Apply smokescreen
                smoke2--; // Decrement available smokescreens
                found = 1; // Mark that a ship has been hidden
                return; // End the turn after using smokescreen
            }
        }
    }

    // Step 3.2: If no ships are found, skip using the smokescreen
    if (!found)
    {
        printf("No ships available to hide with Smoke.\n");
    }
}

    // Step 3: Use radar sweep after 7 misses
    if (countturnsbot >= 6 && radarUsed < 3 && isQueueEmpty() == 1 && isQueueEmptyhori() == 1 && isQueueEmptyvert() == 1)
{
    printf("Bot deciding whether to use Radar Sweep...\n");

    int row = -1, col = -1, validArea = 0;

    // Search for a valid 2x2 area where all cells are water
    for (int i = 0; i < Grid_size - 1 && !validArea; i++) // Prevent out-of-bounds for 2x2
    {
        for (int j = 0; j < Grid_size - 1 && !validArea; j++)
        {
            if (trackingGrid[i][j] == water && trackingGrid[i][j + 1] == water &&
                trackingGrid[i + 1][j] == water && trackingGrid[i + 1][j + 1] == water)
            {
                row = i;
                col = j;
                validArea = 1;
            }
        }
    }

    if (validArea)
    {
        // Found a valid 2x2 water area
        printf("Radar Sweep targeting 2x2 area at %c%d...\n", col + 'A', row + 1);
        radar_sweep_and_process(row + 1, col + 'A', opponentGrid, trackingGrid);
        radarUsed++;
        countturnsbot = 0; // Reset miss count
        return; // End turn after using radar
    }
    else
    {
        // No valid 2x2 water area found
        printf("No valid 2x2 water area found for Radar Sweep.\n");
    }
}

   while (!isQueueEmpty() || !isQueueEmptyhori() || !isQueueEmptyvert())
    {

        // Step 1: Process Horizontal Queue
        if (!isQueueEmptyhori())
        {
            Target hor = horizontal_dequeue();
            while (!validate(hor.row, hor.col) || trackingGrid[hor.row][hor.col] != water)
            {
                if (isQueueEmptyhori())
                    return; // No valid targets in horizontal queue
                hor = horizontal_dequeue();
            }

            printf("Bot firing at %c%d from horizontal queue...\n", hor.col + 'A', hor.row + 1);
            fire(opponentGrid, trackingGrid, hor.row + 1, hor.col + 'A');

            if (trackingGrid[hor.row][hor.col] == hit)
            {
                enqueue_if_valid_horizontal(hor.row, hor.col - 1, trackingGrid); // Left
                enqueue_if_valid_horizontal(hor.row, hor.col + 1, trackingGrid); // Right
            }
            return;
        }

        // Step 2: Process Vertical Queue
        if (!isQueueEmptyvert())
        {
            Target ver = vertical_dequeue();
            while (!validate(ver.row, ver.col) || trackingGrid[ver.row][ver.col] != water)
            {
                if (isQueueEmptyvert())
                    return; // No valid targets in vertical queue
                ver = vertical_dequeue();
            }

            printf("Bot firing at %c%d from vertical queue...\n", ver.col + 'A', ver.row + 1);
            fire(opponentGrid, trackingGrid, ver.row + 1, ver.col + 'A');

            if (trackingGrid[ver.row][ver.col] == hit)
            {
                enqueue_if_valid_vertical(ver.row - 1, ver.col, trackingGrid); // Up
                enqueue_if_valid_vertical(ver.row + 1, ver.col, trackingGrid); // Down
            }
            return;
        }

        // Step 3: Process General Queue
        if (!isQueueEmpty())
        {
            Target t = dequeue();
            while (!validate(t.row, t.col) || trackingGrid[t.row][t.col] != water)
            {
                if (isQueueEmpty())
                    return; // No valid targets in general queue
                t = dequeue();
            }

            printf("Bot firing at %c%d from general queue...\n", t.col + 'A', t.row + 1);
            fire(opponentGrid, trackingGrid, t.row + 1, t.col + 'A');

            if (trackingGrid[t.row][t.col] == hit)
            {
                if (currentTarget.processing == 0)
                {
                    // First hit, initialize targeting
                    currentTarget.row = t.row;
                    currentTarget.col = t.col;
                    currentTarget.processing = 1;
                }
                else if (currentTarget.orientation == 0)
                {
                    // Determine orientation on second hit
                    if (t.row == currentTarget.row)
                    {
                        currentTarget.orientation = 1; // Horizontal
                        printf("\nHORIZONTAL ORIENTATION\n");
                        enqueue_if_valid_horizontal(t.row, t.col - 1, trackingGrid); // Left
                        enqueue_if_valid_horizontal(t.row, t.col + 1, trackingGrid); // Right
                    }
                    else if (t.col == currentTarget.col)
                    {
                        currentTarget.orientation = 2; // Vertical
                        printf("\nVERTICAL ORIENTATION\n");
                        enqueue_if_valid_vertical(t.row - 1, t.col, trackingGrid); // Up
                        enqueue_if_valid_vertical(t.row + 1, t.col, trackingGrid); // Down
                    }
                }
                else
                {
                    // Already have orientation; enqueue accordingly
                    if (currentTarget.orientation == 1) // Horizontal
                    {
                        enqueue_if_valid_horizontal(t.row, t.col - 1, trackingGrid); // Left
                        enqueue_if_valid_horizontal(t.row, t.col + 1, trackingGrid); // Right
                    }
                    else if (currentTarget.orientation == 2) // Vertical
                    {
                        enqueue_if_valid_vertical(t.row - 1, t.col, trackingGrid); // Up
                        enqueue_if_valid_vertical(t.row + 1, t.col, trackingGrid); // Down
                    }
                }
            }
            return;
        }
    }
    // Step 4: Random Targeting
    bot_fire_random_easy_medium(opponentGrid, trackingGrid);
}


void bot_play_turn(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10])
{
    printf("Bot is taking its turn...\n");

    if (diffBot == 0) // Easy Bot
    {
        bot_easy_play(opponentGrid, trackingGrid,botGrid);
    }
    else if (diffBot == 1) // Medium Bot
    {
        bot_medium_play(opponentGrid, trackingGrid,botGrid);
    }
    else if (diffBot == 2) // Hard Bot
    {
        bot_hard_play(opponentGrid, trackingGrid, botGrid); // Existing implementation
    }
}



int main()
{
    create_grid(grid1);
    create_grid(grid2);
    create_grid(gridplayer1);
    create_grid(gridplayer2);
    create_grid(show);
    difficultyLevel();

    char player1[50];
    printf("Enter your name: ");
    scanf("%s", player1);
    while (getchar() != '\n')
        ;

    char player2[] = "Bot";

    // Player places their ships
    printf("%s, please enter your ships:\n", player1);
    print_Grid(gridplayer1);
    place_ships(grid1);
    print_Grid(grid1);
    sleep(3);
    // clear_screen();
    difficultyLevelBot();
    // Bot places its ships
    printf("The bot is placing its ships...\n");
    // print_Grid(gridplayer2);
    place_ships_randomly(grid2);
    print_Grid(grid2);
    sleep(3);
    // clear_screen();

    // Determine who goes first
    currentPlayer = firstPlayer(player1, player2);

    char input[50];
    char move[20], column;
    int row;
    int radar1_count = 3;
    int smoke1 = 0;
    int ships1_sunked = 0;
    int ships2_sunked = 0;
    int artillery1_flag = 0;

    int istorpido1 = 0;

    int once1 = 1;
    int once2 = 1;
    int a1;
    int x =1;

    // Main game loop
        while (ships1_sunked != 4 && ships2_sunked != 4)
        {
            // clear_screen();

            if (currentPlayer == 1)
            {

                print_Grid(gridplayer2);
                printf("Available moves: \nFire: available, Torpedo: %d, Smoke: %d, Artillery: %d, Radar: %d\n",
                    istorpido1, smoke1, artillery1_flag, radar1_count);
                printf("%s, enter your move: ", currentPlayer == 1 ? player1 : player2);
                // while (getchar() != '\n');
                
                fgets(input, sizeof(input), stdin);
                
                sscanf(input, "%s", move);
                for (int i = 0; move[i] != '\0'; i++)
                {
                    move[i] = tolower(move[i]);
                }
                if (strcmp(move, "torpedo") == 0)
                {
                    if (sscanf(input, "%s %d", move, &row) == 2)
                    {
                        // Row is already a number, no further processing needed
                    }
                    else if (sscanf(input, "%s %c", move, &column) == 2)
                    {
                        column = toupper(column); // Normalize column input to uppercase
                        if (column >= 'A' && column <= 'J')
                        {
                            // Convert column letter to column index (0-based)
                            row = column; // Pass the column as-is to the torpedo function
                        }
                        else
                        {
                            printf("Invalid column input for torpedo. Try again.\n");
                            continue; // Skip turn on invalid input
                        }
                    }
                    else
                    {
                        printf("Invalid input format for torpedo. Try again.\n");
                        continue; // Skip turn on invalid input
                    }
                }
                else
                {
                    sscanf(input, "%s %c%d", move, &column, &row);
                }

                for (int i = 0; move[i] != '\0'; i++)
                {
                    move[i] = tolower(move[i]);
                }
                if (strcmp(move, "fire") == 0)
                {

                    artillery1_flag = 0;
                    istorpido1 = 0;
                    fire(grid2, gridplayer2, row, column);
                    print_Grid(gridplayer2);
                }
                else if (strcmp(move, "radar") == 0)
                {
                    if (radar1_count > 0)
                    {
                        artillery1_flag = 0;
                        istorpido1 = 0;
                        radar_sweep(row, column, grid2);
                        radar1_count--;
                    }
                    else
                    {
                        printf("You have used all 3 radar sweeps. :(\n");
                    }
                }
                else if (strcmp(move, "smoke") == 0)
                {
                    if (smoke1 >= 1)
                    {
                        smokeScreen(grid2, row, column);
                        smoke1--;
                    }
                    else
                    {
                        printf("You have used all available smoke screens. :( :( \n");
                    }
                }
                else if (strcmp(move, "artillery") == 0 && artillery1_flag)
                {
                    artillery(grid2, gridplayer2, row, column);
                    print_Grid(gridplayer2);
                    artillery1_flag = 0;
                    istorpido1 = 0;
                }
                else if (strcmp(move, "torpedo") == 0) // && torpedoAvailable
                {
                    torpedo(grid2, gridplayer2, row);
                    print_Grid(gridplayer2);
                    istorpido1 = 0;
                }
                else
                {
                    printf("Invalid move. Try again :(\n");
                    sleep(2);
                    continue;
                }
            }
            else
            {
                if (diff == 0)
                    print_Grid(gridplayer1);
                else
                    print_Grid(show);

                printf("Bot is making its move...\n");
                sleep(2);
                // if(x==1){
                //     radar_sweep_and_process_easy_medium(3,'C',grid1,gridplayer1);x=0;
                // }
                bot_play_turn(grid1, gridplayer1, grid2);
                countturnsbot++;
            }

            if (currentPlayer == 1)
            {
                a1 = sunk_ships(grid2);
                if (a1 > 0)
                {
                    smoke1 += a1;
                    ships2_sunked += a1;
                    artillery1_flag = 1;
                }
            }
            else
            {
                int a2 = sunk_ships(grid1);
                if (a2 > 0)
                {
                    smoke2 += a2;
                    ships1_sunked += a2;
                    artilleryAvailable = 1;
                    // while (!isQueueEmptyhori())
                    // {
                    //     horizontal_dequeue();
                    // }
                    // while (!isQueueEmptyvert())
                    // {
                    //     vertical_dequeue();
                    // }
                     currentTarget.processing=0;
                     currentTarget.orientation = 0;
                }
            }

            if (ships2_sunked == 3 && once1)
            {
                istorpido1 = 1;
                artillery1_flag = 0;
                once1 = 0;
            }
            if (ships1_sunked == 3 && once2)
            {
                torpedoAvailable = 1;
                artilleryAvailable = 0;
                once2 = 0;
            }

            sleep(3);
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
        }

    if (ships1_sunked == 4)
    {
        printf("The bot won!!! Better luck next time, %s.\n", player1);
    }
    else
    {
        printf("Congratulations, %s! You won!!!\n", player1);
    }
    return 0;
}