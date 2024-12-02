
#ifndef FUNCTION_HEADERS_H
#define FUNCTION_HEADERS_H


int contains(char available_ships[], char letter);
int sunk_ships(char grid[10][10]);
int traverse_2d_array(char grid[10][10], int letter);
void print_Grid(char grid[10][10]);
void create_grid(char grid[10][10]);
void difficultyLevel();
int check_coordinates(int row, int col, int size, char orientation, char grid[10][10]);
void place_ships(char grid[10][10]);
int firstPlayer(char player1[], char player2[]);

int validate(int row, int col);
void fire(char grid[10][10], char grid2[10][10], int row, char column);
void radar_sweep(int row, char column, char grid[10][10]);
void torpedo(char grid[10][10], char grid2[10][10], int target);
void smokeScreen(char grid[10][10], int row, char column);
void artillery(char grid[10][10], char grid2[10][10], int row, char column);
void vertical_enqueue(int row, int col);
void enqueue(int row, int col);
void horizontal_enqueue(int row, int col);
int isQueueEmptyvert();
int isQueueEmpty();
int isQueueEmptyhori();
int row_has_untargeted_cells(int row, char trackingGrid[10][10]);
void enqueue_if_valid_vertical(int row, int col, char trackingGrid[10][10]);
void enqueue_if_valid_horizontal(int row, int col, char trackingGrid[10][10]);
void enqueue_if_valid(int row, int column, char trackingGrid[10][10]);
void artilleryBot(char grid[10][10], char grid2[10][10], int row, char column);
void enqueue_adjacent_cells(int row, int col, char trackingGrid[10][10]);
int column_has_untargeted_cells(char col, char trackingGrid[10][10]);
int row_has_untargeted_cells(int row, char trackingGrid[10][10]);
void artilleryBotEasy(char grid[10][10], char grid2[10][10], int row, char column);
void bot_fire_random_until_hit(char opponentGrid[10][10], char trackingGrid[10][10]);
void bot_hard_play(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10]);
void radar_sweep_and_process(int row, char column, char opponentGrid[10][10], char trackingGrid[10][10]);
void torpedoBot(char grid[10][10], char grid2[10][10], int target);
void bot_play_turn(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10]);
void bot_medium_play(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10]);
void bot_easy_play(char opponentGrid[10][10], char trackingGrid[10][10], char botGrid[10][10]);
void place_ships_randomly(char grid[10][10]);
void bot_fire_random_easy_medium(char opponentGrid[10][10], char trackingGrid[10][10]);

void clear_screen();

#endif
