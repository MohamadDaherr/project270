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
void process_queues(char opponentGrid[10][10], char trackingGrid[10][10]);
void bot_fire_with_heatmap(char opponentGrid[10][10], char trackingGrid[10][10]);

int diff;
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

void difficultyLevel()
{
    char difficulty[10];
    while (1)
    {
        printf("Choose difficulty level (Easy or Hard): ");
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
int bot_hits = 0, bot_misses = 0;

void fire(char grid[10][10], char grid2[10][10], int row, char column) {
    int col = toupper(column) - 'A';
    row--;
    if (validate(row, col)) {
        if (grid[row][col] == 'C' || grid[row][col] == 'D' || grid[row][col] == 'S' || grid[row][col] == 'B') {
            grid[row][col] = hit;
            grid2[row][col] = hit;
            bot_hits++;
        } else {
            grid[row][col] = miss;
            grid2[row][col] = miss;
            bot_misses++;
        }
    }
}

void print_bot_stats() {
    printf("Bot Stats: Hits = %d, Misses = %d, Accuracy = %.2f%%\n", 
           bot_hits, bot_misses, bot_hits * 100.0 / (bot_hits + bot_misses));
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

typedef struct {
    int row, col;
    int priority; // Higher value = higher priority
} QueueItem;

QueueItem queue[MAX_QUEUE_SIZE];
int front4 = -1, rear4 = -1;

void enqueue_with_priority(int row, int col, int priority) {
    if (rear4 == MAX_QUEUE_SIZE - 1)
        return;
    QueueItem item = {row, col, priority};
    if (front4 == -1)
        front4 = 0;
    queue[++rear4] = item;
    // Sort queue by priority (simple insertion sort for this example)
    for (int i = rear4; i > front4 && queue[i].priority > queue[i - 1].priority; i--) {
        QueueItem temp = queue[i];
        queue[i] = queue[i - 1];
        queue[i - 1] = temp;
    }
}

QueueItem dequeue_with_priority() {
    if (front4 == -1 || front4 > rear4)
        return (QueueItem){-1, -1, -1};
    return queue[front4++];
}

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
    if (validate(row, column) && trackingGrid[row][column] == water)
    {
        enqueue(row, column);
    }
}
void enqueue_if_valid_horizontal(int row, int col, char trackingGrid[10][10])
{
    if (validate(row, col) && trackingGrid[row][col] == water)
    {
        horizontal_enqueue(row, col);
    }
}

void enqueue_if_valid_vertical(int row, int col, char trackingGrid[10][10])
{
    if (validate(row, col) && trackingGrid[row][col] == water)
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
    if (validate(row - 1, col) && trackingGrid[row - 1][col] == water)
    {
        enqueue(row - 1, col);
    }
    if (validate(row, col - 1) && trackingGrid[row][col - 1] == water)
    {
        enqueue(row, col - 1);
    }
    if (validate(row + 1, col) && trackingGrid[row + 1][col] == water)
    {
        enqueue(row + 1, col);
    }
    if (validate(row, col + 1) && trackingGrid[row][col + 1] == water)
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

    for (int i = row; i < row + 2 && i < Grid_size; i++)
    {
        for (int j = col; j < col + 2 && j < Grid_size; j++)
        {
            if (grid[i][j] == 'C' || grid[i][j] == 'D' || grid[i][j] == 'S' || grid[i][j] == 'B')
            {
                grid[i][j] = hit;
                grid2[i][j] = hit;
                printf("Hit at %c%d!\n", j + 'A', i + 1);
                enqueue_adjacent_cells(i, j, grid2);
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
void radar_sweep_and_process(int row, char column, char opponentGrid[10][10], char trackingGrid[10][10])
{
    printf("Bot is scanning a 2x2 area with Radar at %c%d...\n", column, row);

    row--;
    column = column - 'A';
    int found = 0;

    for (int i = row; i < row + 2 && i < Grid_size; i++)
    {
        for (int j = column; j < column + 2 && j < Grid_size; j++)
        {
            if (validate(i, j))
            {
                // Mark radar-swept cells
                if (opponentGrid[i][j] == water)
                {
                    opponentGrid[i][j] = 'r'; // Mark cell as radar-swept
                }

                // Enqueue ships detected by radar
                if (opponentGrid[i][j] == 'C' || opponentGrid[i][j] == 'D' || opponentGrid[i][j] == 'S' || opponentGrid[i][j] == 'B')
                {
                    enqueue_if_valid(i, j, trackingGrid);
                    enqueue_adjacent_cells(i, j, trackingGrid);
                    printf("Ship detected at %c%d\n", j + 'A', i + 1);
                    found = 1;
                }
            }
        }
    }

    if (!found)
    {
        printf("No enemy ships found in the scanned area.\n");
    }
}
void process_queues(char opponentGrid[10][10], char trackingGrid[10][10]) {
    // Process horizontal queue
    if (!isQueueEmptyhori()) {
        Target hor = horizontal_dequeue();
        if (validate(hor.row, hor.col) && trackingGrid[hor.row][hor.col] == water) {
            printf("Bot firing at %c%d from horizontal queue...\n", hor.col + 'A', hor.row + 1);
            fire(opponentGrid, trackingGrid, hor.row + 1, hor.col + 'A');
            return; // End turn after firing from horizontal queue
        }
    }

    // Process vertical queue
    if (!isQueueEmptyvert()) {
        Target ver = vertical_dequeue();
        if (validate(ver.row, ver.col) && trackingGrid[ver.row][ver.col] == water) {
            printf("Bot firing at %c%d from vertical queue...\n", ver.col + 'A', ver.row + 1);
            fire(opponentGrid, trackingGrid, ver.row + 1, ver.col + 'A');
            return; // End turn after firing from vertical queue
        }
    }

    // Process general queue
    if (!isQueueEmpty()) {
        Target gen = dequeue();
        if (validate(gen.row, gen.col) && trackingGrid[gen.row][gen.col] == water) {
            printf("Bot firing at %c%d from general queue...\n", gen.col + 'A', gen.row + 1);
            fire(opponentGrid, trackingGrid, gen.row + 1, gen.col + 'A');
            return; // End turn after firing from general queue
        }
    }
}


void bot_fire_with_heatmap(char opponentGrid[10][10], char trackingGrid[10][10]) {
    // Logic to fire at the most promising locations based on a heatmap
    int heatmap[10][10] = {0};

    // Generate heatmap based on the remaining ship positions
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (trackingGrid[i][j] == water) {
                heatmap[i][j] += 1;  // Example: increment heatmap score
            }
        }
    }

    // Find the best target based on the heatmap
    int maxScore = 0;
    int targetRow = 0, targetCol = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (heatmap[i][j] > maxScore) {
                maxScore = heatmap[i][j];
                targetRow = i;
                targetCol = j;
            }
        }
    }

    // Fire at the target with the highest score
    printf("Bot firing at %c%d using heatmap...\n", targetCol + 'A', targetRow + 1);
    fire(opponentGrid, trackingGrid, targetRow + 1, targetCol + 'A');
}
void bot_play_turn(char opponentGrid[10][10], char trackingGrid[10][10]) {
    printf("Bot is taking its turn...\n");

    // Step 1: Use torpedo if available
    if (torpedoAvailable) {
        printf("Bot using Torpedo...\n");
        int axis = rand() % 2; // 0 for row, 1 for column
        if (axis == 0) {
            int row;
            do {
                row = rand() % Grid_size + 1; // Rows are 1-indexed
            } while (!row_has_untargeted_cells(row, trackingGrid)); // Ensure row has untargeted cells
            torpedo(opponentGrid, trackingGrid, row);
        } else {
            char col;
            do {
                col = rand() % Grid_size + 'A'; // Columns are A-J
            } while (!column_has_untargeted_cells(col, trackingGrid)); // Ensure column has untargeted cells
            torpedo(opponentGrid, trackingGrid, col);
        }
        print_Grid(trackingGrid);
        torpedoAvailable = 0; // Torpedo is one-time use
        return;               // End the turn after using torpedo
    }

    // Step 2: Use artillery if available
    if (artilleryAvailable) {
        printf("Bot using Artillery...\n");
        int row, col;
        do {
            row = rand() % (Grid_size - 1);
            col = rand() % (Grid_size - 1);
        } while (trackingGrid[row][col] != water); // Ensure the cell is untargeted

        artilleryBot(opponentGrid, trackingGrid, row + 1, col + 'A');
        print_Grid(trackingGrid);
        artilleryAvailable = 0; // Artillery is one-time use
        return;                 // End the turn after using artillery
    }

    // Step 3: Use smoke if available
    if (smoke2 > 0 && isQueueEmpty() && isQueueEmptyhori() && isQueueEmptyvert()) {
        int row, col;
        do {
            row = rand() % Grid_size; // Random row
            col = rand() % Grid_size; // Random column
        } while (!validate(row, col)); // Ensure the coordinates are valid

        printf("Bot is using Smoke at %c%d...\n", col + 'A', row + 1);
        smokeScreen(opponentGrid, row + 1, col + 'A'); // Apply smoke to a 2x2 area
        smoke2--;                                      // Decrement the number of available smokes for the bot
        return;                                        // End the turn after using smoke
    }

    // Step 4: Use radar sweep after a set number of misses
    if (countturnsbot >= 6 && radarUsed < 3 && isQueueEmpty() && isQueueEmptyhori() && isQueueEmptyvert()) {
        printf("Bot using Radar Sweep...\n");
        int row, col;
        do {
            row = rand() % (Grid_size - 1);
            col = rand() % (Grid_size - 1);
        } while (trackingGrid[row][col] != water); // Ensure the cell is untargeted

        radar_sweep_and_process(row + 1, col + 'A', opponentGrid, trackingGrid);
        radarUsed++;
        countturnsbot = 0; // Reset miss count
        return;            // End the turn after using radar
    }

    // Step 5: Process targeting queues (horizontal, vertical, or general)
    if (!isQueueEmpty() || !isQueueEmptyhori() || !isQueueEmptyvert()) {
        process_queues(opponentGrid, trackingGrid);
        return; // End the turn after processing a queue
    }

    // Step 6: Random targeting using heatmap
    printf("Bot firing using Heatmap...\n");
    bot_fire_random_until_hit(opponentGrid, trackingGrid);
    return; // End the turn after firing randomly
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
int heatmap[10][10] = {0};

// Update the heatmap based on possible placements of remaining ships
void update_heatmap(char grid[10][10]) {
    memset(heatmap, 0, sizeof(heatmap));

    for (int i = 0; i < Grid_size; i++) {
        for (int j = 0; j < Grid_size; j++) {
            if (grid[i][j] == water) {
                // Check horizontal and vertical placements for each remaining ship
                for (int k = 0; k < total_ships; k++) {
                    if (ships[k].size > 0) { // Only consider unsunk ships
                        if (check_coordinates(i, j, ships[k].size, 'H', grid))
                            for (int l = 0; l < ships[k].size; l++)
                                heatmap[i][j + l]++;

                        if (check_coordinates(i, j, ships[k].size, 'V', grid))
                            for (int l = 0; l < ships[k].size; l++)
                                heatmap[i + l][j]++;
                    }
                }
            }
        }
    }
}

// Choose the cell with the highest heatmap value for random firing

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
\
    // Bot places its ships
    printf("The bot is placing its ships...\n");
    place_ships_randomly(grid2);
    print_Grid(grid2); // Show bot's grid for debugging; remove this in the final version.
    sleep(3);

    // Determine who goes first
    currentPlayer = firstPlayer(player1, player2);

    char input[50];
    char move[20], column;
    int row;
    int radar1_count = 3;  // Player radar sweeps
    int smoke1 = 0;        // Player smoke screens
    int ships1_sunked = 0; // Bot's sunk ships
    int ships2_sunked = 0; // Player's sunk ships
    int artillery1_flag = 0;

    int istorpido1 = 0;
    int once1 = 1, once2 = 1;

    printf("Game starts now! Good luck!\n");

    // Main game loop
    while (ships1_sunked != total_ships && ships2_sunked != total_ships)
    {
        // clear_screen();

        if (currentPlayer == 1)
        {
            print_Grid(gridplayer2);
            printf("Available moves:\nFire: available, Torpedo: %d, Smoke: %d, Artillery: %d, Radar: %d\n",
                   istorpido1, smoke1, artillery1_flag, radar1_count);
            printf("%s, enter your move: ", player1);

            fgets(input, sizeof(input), stdin);
            sscanf(input, "%s", move);
            for (int i = 0; move[i] != '\0'; i++)
                move[i] = tolower(move[i]);

            if (strcmp(move, "torpedo") == 0)
            {
                if (sscanf(input, "%s %d", move, &row) == 2 || sscanf(input, "%s %c", move, &column) == 2)
                {
                    torpedo(grid2, gridplayer2, row ? row : column);
                    istorpido1 = 0;
                }
                else
                {
                    printf("Invalid torpedo input! Try again.\n");
                    continue;
                }
            }
            else if (strcmp(move, "fire") == 0)
            {
                sscanf(input, "%s %c%d", move, &column, &row);
                fire(grid2, gridplayer2, row, column);
            }
            else if (strcmp(move, "radar") == 0 && radar1_count > 0)
            {
                sscanf(input, "%s %c%d", move, &column, &row);
                radar_sweep(row, column, grid2);
                radar1_count--;
            }
            else if (strcmp(move, "smoke") == 0 && smoke1 > 0)
            {
                sscanf(input, "%s %c%d", move, &column, &row);
                smokeScreen(grid2, row, column);
                smoke1--;
            }
            else if (strcmp(move, "artillery") == 0 && artillery1_flag)
            {
                sscanf(input, "%s %c%d", move, &column, &row);
                artillery(grid2, gridplayer2, row, column);
                artillery1_flag = 0;
            }
            else
            {
                printf("Invalid move! Try again.\n");
                continue;
            }

            // Check if bot's ships were sunk
            int sunked = sunk_ships(grid2);
            if (sunked > 0)
            {
                smoke1 += sunked;
                ships2_sunked += sunked;
                artillery1_flag = 1;
            }
        }
        else
        {                    print_Grid(gridplayer1);

            // Bot's turn
            printf("Bot is taking its turn...\n");
            sleep(2);
            bot_play_turn(grid1, gridplayer1);

            // Check if player's ships were sunk
            int sunked = sunk_ships(grid1);
            if (sunked > 0)
            {
                smoke2 += sunked;
                ships1_sunked += sunked;
                artilleryAvailable = 1;

                // Reset bot targeting queues for new ships
                while (!isQueueEmptyhori())
                    horizontal_dequeue();
                while (!isQueueEmptyvert())
                    vertical_dequeue();
                currentTarget.processing = 0;
            }
        }

        // Enable torpedoes and artillery when 3 ships are sunk
        if (ships2_sunked == total_ships - 1 && once1)
        {
            istorpido1 = 1;
            once1 = 0;
        }
        if (ships1_sunked == total_ships - 1 && once2)
        {
            torpedoAvailable = 1;
            once2 = 0;
        }

        currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch turn
    }

    // Declare winner
    if (ships1_sunked == total_ships)
    {
        printf("The bot won! Better luck next time, %s.\n", player1);
    }
    else
    {
        printf("Congratulations, %s! You defeated the bot!\n", player1);
    }

    return 0;
}
