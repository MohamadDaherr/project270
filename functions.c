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
    return 1;
}

void place_ships(char grid[10][10])
{
    for (int i = 0; i < total_ships; i++)
    {
        int row, col;
        char orientation;
        int valid = 1;

        while (valid)
        {
            printf("Place your %s (size %d).\n", ships[i].name, ships[i].size);
            printf("Enter column (A_J), row (1-10), and orientation (H for horizontal, V for vertical): ");

            // This variable to convert from letter to the exact index
            char column;
            scanf(" %c %d %c", &column, &row, &orientation);
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
                    else
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

int done()
{
    char input;
    // Loop until the user enters 'd'
    while (1)
    {
        printf("Enter 'd' when you are done: ");
        scanf(" %c", &input);
        input = tolower(input);
        if (input == 'd')
        {
            printf("Thank you! You entered 'd'.\n");
            return 1;
        }
        else
        {
            printf("Invalid input. Please enter 'd' to indicate you are done.\n");
        }
    }
}

int validate(int row, int col)
{
    if (col < 0 || col >= Grid_size || row < 0 || row >= Grid_size)
    {
        printf("ERROR YA HMAR \n");
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
void torpedo(char grid[10][10], char grid2[10][10], char target)
{
    int is_column = isalpha(target);

    if (is_column)
    {
        int col = toupper(target) - 'A';

        if (col < 0 || col >= Grid_size)
        {
            printf("Invalid column. You lose your turn.\n");
            return;
        }

        for (int row = 0; row < Grid_size; row++)
        {
            fire(grid, grid2, row + 1, target);
        }
    }
    else
    {
        int row = target - '1';
        if (row < 0 || row >= Grid_size)
        {
            printf("Invalid row. You lose your turn.\n");
            return;
        }

        for (int col = 0; col < Grid_size; col++)
        {
            fire(grid, grid2, row + 1, col + 'A');
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

int main()
{
    char grid1[10][10], grid2[10][10];             // Ship grids for Player 1 and Player 2
    char gridplayer1[10][10], gridplayer2[10][10]; // Tracking grids for hits/misses

    // Initialize all grids
    create_grid(grid1);
    create_grid(grid2);
    create_grid(gridplayer1);
    create_grid(gridplayer2);

    // Choose difficulty level (Easy or Hard)
    difficultyLevel();

    char player1[50], player2[50];

    // Get player names
    printf("Enter name of Player 1: ");
    scanf("%s", player1);
    printf("Enter name of Player 2: ");
    scanf("%s", player2);

    // Both players place their ships
    printf("%s, please enter your ships:\n", player1);
    place_ships(grid1);
    printf("%s, please enter your ships:\n", player2);
    place_ships(grid2);

    // Determine who goes first
    currentPlayer = firstPlayer(player1, player2);

    char input[50];
    char move[20], column;
    int row;
    int radar1_count = 3;
    int radar2_count = 3;
    int smoke1 = 0;
    int smoke2 = 0;
    int ships1_sunked = 0;
    int ships2_sunked = 0;
    int artillery1_flag = 0;
    int artillery2_flag = 0;

    int istorpido1 = 0;
    int istorpido2 = 0;
    int torpedo1 = 0;
    int torpedo2 = 0;

    int once1 = 1;
    int once2 = 1;
    int a1;
    

    // Main game loop
    while (ships1_sunked != 4 && ships2_sunked != 4)
    {
        // Prompt the current player to enter their move and location (e.g., "Fire B3" or "Radar A1")
        printf("%s, enter your move: ", currentPlayer == 1 ? player1 : player2);
        fgets(input, sizeof(input), stdin); // Read the whole input line

        // Parse the input: first the move (string), then the column (char) and row (int)
        sscanf(input, "%s %c%d", move, &column, &row);
        for (int i = 0; move[i] != '\0'; i++)
        {
            move[i] = tolower(move[i]);
        } // Convert the move to lowercase for case-insensitivity

        // Perform the corresponding action based on the move
        if (strcmp(move, "fire") == 0)
        {
            // Fire move


            if (currentPlayer == 1)
            {
                artillery1_flag = 0;
                istorpido1 = 0;

                fire(grid2, gridplayer2, row, column); // Player 1 fires at Player 2's grid
                print_Grid(gridplayer2);
            }
            else
            {
                artillery2_flag = 0;
                istorpido2 = 0;
                fire(grid1, gridplayer1, row, column); // Player 2 fires at Player 1's grid
                print_Grid(gridplayer1);
            }
        }
        else if (strcmp(move, "radar") == 0)
        {
            // Radar sweep
            if (radar1_count > 0)
            {
                if (currentPlayer == 1)
                {
                    artillery1_flag = 0;
                    istorpido1 = 0;
                    radar_sweep(row, column, grid2); // Player 1 performs radar sweep on Player 2's grid
                    radar1_count--;
                }
                else
                {
                    artillery2_flag = 0;
                    istorpido2 = 0;
                    radar_sweep(row, column, grid1); // Player 2 performs radar sweep on Player 1's grid
                    radar2_count--;
                }
            }
            else
            {
                printf("You have used all 3 radar sweeps.\n");
            }
        }
        else if (strcmp(move, "smoke") == 0)
        {
            // Smoke screen
            { // Allow 1 smoke screen for now, can adjust based on sunk ships

                if (currentPlayer == 1)
                {
                    artillery1_flag = 0;
                    istorpido1 = 0;
                    if (smoke1 >= 1)
                    {
                        smokeScreen(grid1, row, column); // Player 1 obscures their own grid
                        smoke1--;
                    }
                    else
                        printf("You have used all available smoke screens.\n");
                }
                else
                {
                    artillery2_flag = 0;
                    istorpido2 = 0;
                    if (smoke2 >= 1)
                    {
                        smokeScreen(grid2, row, column); // Player 2 obscures their own grid
                        smoke2--;
                    }
                    else
                        printf("You have used all available smoke screens.\n");
                }
            }
        }
        else if (strcmp(move, "artillery") == 0)
        {
            // Artillery move
            if (currentPlayer == 1 && artillery1_flag)
            {
                artillery(grid2, gridplayer2, row, column); // Player 1 fires artillery at Player 2's grid
                print_Grid(gridplayer2);
                artillery1_flag = 0;
                istorpido1 = 0;
            }
            else if (currentPlayer == 2 && artillery2_flag)
            {
                artillery(grid1, gridplayer1, row, column); // Player 2 fires artillery at Player 1's grid
                print_Grid(gridplayer1);
                artillery2_flag = 0;
                istorpido2 = 0;
            }
            else
                printf("No available moves.\n");
        }
        else if (strcmp(move, "torpedo") == 0)
        {
            // Torpedo move (entire row or column)
            if (currentPlayer == 1 && istorpido1)
            {// printf("%d \n", istorpido1);
                artillery1_flag = 0;
                torpedo(grid2, gridplayer2, column); // Player 1 fires torpedo at Player 2's grid
                print_Grid(gridplayer2);
                istorpido1 = 0;
            }
            else if (currentPlayer == 2 && istorpido2)
            {
                artillery2_flag = 0;
                torpedo(grid1, gridplayer1, column); // Player 2 fires torpedo at Player 1's grid
                print_Grid(gridplayer1);
                istorpido2 = 0;
            }
            else
                printf("No available move");
        }
        else
        {
            printf("Invalid move. Try again.\n");
            continue; // Ask for the next move
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
                artillery2_flag = 1;
            }
        }
   
        if (ships2_sunked == 3 && once1)
        {
          //  torpedo1++;
            istorpido1 = 1;
            artillery1_flag = 0;
            once1 = 0;
        }
        if (ships1_sunked == 3 && once2)
        {
            torpedo2++;
            istorpido2 = 1;
            artillery2_flag = 0;
            once2 = 0;
                        printf("torpido unloacked");

        }
                        printf("%d\n",istorpido1);


        // Switch turns
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    return 0;
}
