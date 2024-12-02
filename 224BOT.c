

#define MAX_QUEUE_SIZE 100


int count_for_grid=0;
int radarUsed = 0;
int smokeScreensUsed = 0;
int artilleryAvailable = 0;
int torpedoAvailable = 0;
int submarineHidden=0;


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

void bot_easy_play(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10])
{

    if (torpedoAvailable)
    {
        printf("Bot using Torpedo...\n");

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
            torpedoBot_easy(opponentGrid, trackingGrid, targetRow);
        }
        else
        {
            printf("Targeting column %c with Torpedo...\n", targetCol);
            torpedoBot_easy(opponentGrid, trackingGrid, targetCol);
        }

        torpedoAvailable = 0; // Torpedo is one-time use
        return; // End turn after using Torpedo
    }

    if (artilleryAvailable)
    {

        int row, col;
        do
        {
            row = rand() % (Grid_size - 1);
            col = rand() % (Grid_size - 1);
        } while (trackingGrid[row][col] != water); // Ensure the cell is untargeted
        printf("Targeting cell (%d, %d) with Artillery...\n", row +1,col+'A');
        artilleryBotEasy(opponentGrid, trackingGrid, row + 1, col + 'A');
        artilleryAvailable = 0;
        return;                 
    }


    if (countturnsbot >= 6 && radarUsed < 3 && isQueueEmpty() == 1)
{
    int row = -1, col = -1;
    int validArea = 0;

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
        printf("Radar Sweep targeting %c%d...\n", col + 'A', row + 1);

        // Call radar sweep process (modify as per your radar logic)
        radar_sweep_and_process(row + 1, col + 'A', opponentGrid, trackingGrid);
        radarUsed++;
        countturnsbot = 0; // Reset miss count
        return; // End turn after using radar
    } 
}

    // Step 4: Use Smoke if available
    if (smoke2 > 1 && artilleryAvailable == 0 && isQueueEmpty() == 1&& isQueueEmptyhori() == 1 && isQueueEmptyvert() == 1)
{
    int found = 0;
    for (int row = 0; row < Grid_size && !found; row++)
    {
        for (int col = 0; col < Grid_size; col++)
        {
            if (botGrid[row][col] == 'C' || botGrid[row][col] == 'B' || botGrid[row][col] == 'D' || botGrid[row][col] == 'S')
            {
                printf("Bot using Smoke");
                smokeScreen(botGrid, row + 1, col + 'A'); 
                smoke2--; 
                found = 1; 
                return; 
            }
        }
    }
}

    while (!isQueueEmpty())
    {
        Target t = dequeue();
        printf("Bot targeting at %c%d\n", t.col + 'A', t.row + 1);

        // Check if the cell is already hit or missed
        if (trackingGrid[t.row][t.col] == hit || trackingGrid[t.row][t.col] == miss)
        {
            continue; 
        }

            fire(opponentGrid, trackingGrid, t.row + 1, t.col + 'A');
            if (trackingGrid[t.row][t.col] == hit)
            {
                enqueue_adjacent_cells(t.row, t.col, trackingGrid);
            }
        return; 
    }
    bot_fire_random_easy_medium(opponentGrid, trackingGrid);
}

void bot_medium_play(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10])
{
   if (torpedoAvailable)
{
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


    if (maxRowWater >= maxColWater)
    {
        printf("Bot using torpedo to target row %d.\n", targetRow);
        torpedoBot(opponentGrid, trackingGrid, targetRow);
    }
    else
    {
        printf("Bot using torpedo to target column %c\n", targetCol + 'A');
        torpedoBot(opponentGrid, trackingGrid, targetCol + 'A'); 
    }

    print_Grid(trackingGrid); 
    torpedoAvailable = 0;     
    return;                  
}

    if (artilleryAvailable)
    {
        int row, col;
        do
        {
            row = rand() % (Grid_size - 1);
            col = rand() % (Grid_size - 1);
        } while (trackingGrid[row][col] != water); // Ensure the cell is untargeted

        artilleryBot(opponentGrid, trackingGrid, row + 1, col + 'A');
        print_Grid(trackingGrid);
        artilleryAvailable = 0;
        return;                 
    }

    if (smoke2 > 1 && artilleryAvailable == 0 && isQueueEmpty() == 1 && isQueueEmptyhori() == 1 && isQueueEmptyvert() == 1)
{

    int found = 0;
    for (int row = 0; row < Grid_size && !found; row++)
    {
        for (int col = 0; col < Grid_size; col++)
        {
            if (botGrid[row][col] == 'C' || botGrid[row][col] == 'B' || botGrid[row][col] == 'D' || botGrid[row][col] == 'S')
            {
                printf("Bot using Smoke");
                smokeScreen(botGrid, row + 1, col + 'A'); 
                smoke2--; 
                found = 1; 
                return; 
            }
        }
    }
}

    if (countturnsbot >= 6 && radarUsed < 3 && isQueueEmpty() == 1 && isQueueEmptyhori() == 1 && isQueueEmptyvert() == 1)
{
    int row = -1, col = -1, validArea = 0;

    for (int i = 0; i < Grid_size - 1 && !validArea; i++) 
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
        printf("Radar Sweep at %c%d...\n", col + 'A', row + 1);
        radar_sweep_and_process(row + 1, col + 'A', opponentGrid, trackingGrid);
        radarUsed++;
        countturnsbot = 0; // Reset miss count
        return; // End turn after using radar
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

            printf("Bot firing at %c%d\n", hor.col + 'A', hor.row + 1);
            fire(opponentGrid, trackingGrid, hor.row + 1, hor.col + 'A');

            if (trackingGrid[hor.row][hor.col] == hit)
            {
                enqueue_if_valid_horizontal(hor.row, hor.col - 1, trackingGrid); // Left
                enqueue_if_valid_horizontal(hor.row, hor.col + 1, trackingGrid); // Right
            }
            return;
        }
        if (!isQueueEmptyvert())
        {
            Target ver = vertical_dequeue();
            while (!validate(ver.row, ver.col) || trackingGrid[ver.row][ver.col] != water)
            {
                if (isQueueEmptyvert())
                    return; 
                ver = vertical_dequeue();
            }

            printf("Bot firing at %c%d\n", ver.col + 'A', ver.row + 1);
            fire(opponentGrid, trackingGrid, ver.row + 1, ver.col + 'A');

            if (trackingGrid[ver.row][ver.col] == hit)
            {
                enqueue_if_valid_vertical(ver.row - 1, ver.col, trackingGrid); // Up
                enqueue_if_valid_vertical(ver.row + 1, ver.col, trackingGrid); // Down
            }
            return;
        }

        if (!isQueueEmpty())
        {
            Target t = dequeue();
            while (!validate(t.row, t.col) || trackingGrid[t.row][t.col] != water)
            {
                if (isQueueEmpty())
                    return; 
                t = dequeue();
            }

            printf("Bot firing at %c%d \n", t.col + 'A', t.row + 1);
            fire(opponentGrid, trackingGrid, t.row + 1, t.col + 'A');

            if (trackingGrid[t.row][t.col] == hit)
            {
                if (currentTarget.processing == 0)
                {
                    currentTarget.row = t.row;
                    currentTarget.col = t.col;
                    currentTarget.processing = 1;
                }
                else if (currentTarget.orientation == 0)
                {
    
                    if (t.row == currentTarget.row)
                    {
                        currentTarget.orientation = 1; 
                        enqueue_if_valid_horizontal(t.row, t.col - 1, trackingGrid); // Left
                        enqueue_if_valid_horizontal(t.row, t.col + 1, trackingGrid); // Right
                    }
                    else if (t.col == currentTarget.col)
                    {
                        currentTarget.orientation = 2; // Vertical
                        enqueue_if_valid_vertical(t.row - 1, t.col, trackingGrid); // Up
                        enqueue_if_valid_vertical(t.row + 1, t.col, trackingGrid); // Down
                    }
                }
                else
                {
                
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
                
            }
            else if (diff == 0 && grid[i][j] == water)
            {
                grid[i][j] = miss;
                grid2[i][j] = miss;
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
        printf("Bot targeting row %d\n", targetRow);
        torpedoBot(opponentGrid, trackingGrid, targetRow); // Fire torpedo at the row
    }
    else
    {
        printf("Bot targeting column %c.\n", targetCol + 'A');
        torpedoBot(opponentGrid, trackingGrid, targetCol + 'A'); // Fire torpedo at the column
    }

    print_Grid(trackingGrid); // Display the tracking grid after firing
    torpedoAvailable = 0;     // Torpedo is one-time use
    return;                   // End the turn after using torpedo
}

    if (artilleryAvailable)
    {

        int row, col;
        do
        {
            row = rand() % (Grid_size - 1);
            col = rand() % (Grid_size - 1);
        } while (trackingGrid[row][col] != water); // Ensure the cell is untargeted

        printf("Targeting cell (%d, %d) with Artillery...\n", row +1,col+'A');
        artilleryBot(opponentGrid, trackingGrid, row + 1, col + 'A');
        artilleryAvailable = 0; 
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
    if (radarUsed < 3 && isQueueEmpty() == 1)
{
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
        printf("Radar Sweep at %c%d...\n", col + 'A', row + 1);

        // Call radar sweep process (modify as per your radar logic)
        radar_sweep_and_process(row + 1, col + 'A', opponentGrid, trackingGrid);

        radarUsed++;
        countturnsbot = 0; // Reset miss count
        return; // End turn after using radar
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

            printf("Bot firing at %c%d\n", hor.col + 'A', hor.row + 1);
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

            printf("Bot firing at %c%d\n", ver.col + 'A', ver.row + 1);
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

            printf("Bot firing at %c%d\n", t.col + 'A', t.row + 1);
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
                        enqueue_if_valid_horizontal(t.row, t.col - 1, trackingGrid); // Left
                        enqueue_if_valid_horizontal(t.row, t.col + 1, trackingGrid); // Right
                    }
                    else if (t.col == currentTarget.col)
                    {
                        currentTarget.orientation = 2; // Vertical
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
            if(count_for_grid<20){
            // Check surrounding cells to ensure no neighbors are hit
            if (trackingGrid[row - 1][col] != water)
                valid = 0; // Up
            if (trackingGrid[row + 1][col] != water)
                valid = 0; // Down
            if (trackingGrid[row][col - 1] != water)
                valid = 0; // Left
            if (trackingGrid[row][col + 1] != water)
                valid = 0; // Right
            }
        }
    }
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