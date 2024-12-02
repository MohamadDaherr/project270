

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

