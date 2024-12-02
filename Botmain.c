#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include "functions.c"
#include "224BOT.c"
#include "function_headers.h"



int main()
{
    char grid1[10][10], grid2[10][10];
    char gridplayer1[10][10], gridplayer2[10][10];

    create_grid(grid1);
    create_grid(grid2);
    create_grid(gridplayer1);
    create_grid(gridplayer2);
    difficultyLevel();

    char player1[50];
    printf("Enter your name: ");
    scanf("%s", player1);
    while (getchar() != '\n');
    char player2[] = "Bot";

    printf("%s, please enter your ships:\n", player1);
    print_Grid(gridplayer1);
    place_ships(grid1);
    print_Grid(grid1);
    sleep(3);
     clear_screen();
    difficultyLevelBot();
    // Bot places its ships
    printf("The bot is placing its ships...\n");
    // print_Grid(gridplayer2);
    place_ships_randomly(grid2);
    //print_Grid(grid2);
    sleep(3);
    clear_screen();

    // Determine who goes first
    currentPlayer = firstPlayer(player1, player2);

    char input[50];
    char move[20], column;
    int row;
    int radar1_count = 3;
    int smoke1 = 0;
    int smoke2 = 0;
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
            clear_screen();

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
            {   print_Grid(gridplayer1);
                // printf("Bot is making its move...\n");
                sleep(2);
                bot_play_turn(grid1, gridplayer1, grid2);
                print_Grid(gridplayer1);
                sleep(3);
                count_for_grid++;
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