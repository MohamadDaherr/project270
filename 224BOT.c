

#define MAX_QUEUE_SIZE 100





int diffBot;

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


void bot_play_turn(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10])
{
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
            if (toupper(grid[i][j]) == 'C' || toupper(grid[i][j]) == 'B' || toupper(grid[i][j]) == 'D' || toupper(grid[i][j]) == 'S')
            {
                grid[i][j] = hit;
                grid2[i][j] = hit;
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
            }
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
                    found = 1;     // A ship part is detected
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
        }
        return; // Exit the function if no ships were found
    }

    if (found)
    {
        printf("Ships detected.\n");
    }
    else
    {
        printf("No enemy ships found in the scanned area.\n");
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