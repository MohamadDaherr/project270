int main() {
    char grid1[10][10], grid2[10][10];  // Ship grids for Player 1 and Player 2
    char gridplayer1[10][10], gridplayer2[10][10];  // Tracking grids for hits/misses
    
    // Initialize all grids
    create_grid(grid1);
    create_grid(grid2);
    create_grid(gridplayer1);
    create_grid(gridplayer2);
    
    // Choose difficulty level (Easy or Hard)
    difficultyLevel();
    
    char player1[50], player2[50];
    int currentPlayer;

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
    int radar_count = 0, smoke_count = 0;

    // Main game loop
    while (1) {
        // Prompt the current player to enter their move and location (e.g., "Fire B3" or "Radar A1")
        printf("%s, enter your move (e.g., Fire B3, Radar A1): ", currentPlayer == 1 ? player1 : player2);
        fgets(input, sizeof(input), stdin);  // Read the whole input line
        
        // Parse the input: first the move (string), then the column (char) and row (int)
        sscanf(input, "%s %c%d", move, &column, &row);
        move[0] = tolower(move[0]);  // Convert the move to lowercase for case-insensitivity

        // Perform the corresponding action based on the move
        if (strcmp(move, "fire") == 0) {
            // Fire move
            if (currentPlayer == 1) {
                fire(grid2, gridplayer1, row, column);  // Player 1 fires at Player 2's grid
                print_Grid(gridplayer1);
            } else {
                fire(grid1, gridplayer2, row, column);  // Player 2 fires at Player 1's grid
                print_Grid(gridplayer2);
            }

        } else if (strcmp(move, "radar") == 0) {
            // Radar sweep
            if (radar_count < 3) {
                if (currentPlayer == 1) {
                    radar_sweep(row, column, grid2);  // Player 1 performs radar sweep on Player 2's grid
                } else {
                    radar_sweep(row, column, grid1);  // Player 2 performs radar sweep on Player 1's grid
                }
                radar_count++;
            } else {
                printf("You have used all 3 radar sweeps.\n");
            }

        } else if (strcmp(move, "smoke") == 0) {
            // Smoke screen
            if (smoke_count < 1) {  // Allow 1 smoke screen for now, can adjust based on sunk ships
                if (currentPlayer == 1) {
                    smokeScreen(grid1, row, column);  // Player 1 obscures their own grid
                } else {
                    smokeScreen(grid2, row, column);  // Player 2 obscures their own grid
                }
                smoke_count++;
            } else {
                printf("You have used all available smoke screens.\n");
            }

        } else if (strcmp(move, "artillery") == 0) {
            // Artillery move
            if (currentPlayer == 1) {
                artillery(grid2, gridplayer1, row, column);  // Player 1 fires artillery at Player 2's grid
                print_Grid(gridplayer1);
            } else {
                artillery(grid1, gridplayer2, row, column);  // Player 2 fires artillery at Player 1's grid
                print_Grid(gridplayer2);
            }

        } else if (strcmp(move, "torpedo") == 0) {
            // Torpedo move (entire row or column)
            if (currentPlayer == 1) {
                torpedo(grid2, gridplayer1, column);  // Player 1 fires torpedo at Player 2's grid
                print_Grid(gridplayer1);
            } else {
                torpedo(grid1, gridplayer2, column);  // Player 2 fires torpedo at Player 1's grid
                print_Grid(gridplayer2);
            }

        } else {
            printf("Invalid move. Try again.\n");
            continue;  // Ask for the next move
        }

        // Switch turns
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    return 0;
}