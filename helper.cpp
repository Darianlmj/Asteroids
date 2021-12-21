#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <time.h>
#include "asteroids.hpp"

using namespace std;

/**
 * Print out the contents of the main map array.
 * Also prints out a ">" symbol to denote the current position of the spacecraft.
 *
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *      int laserY  - Position of spacecraft.
 *  Returns:
 *      -
 */
void print_map(vector<vector<int> > *map, int laserY) {
    for (int row = 0; row < MAP_SIZE; row++) {  
        if (row == laserY) {      
            cout << "> ";
        } else {
            cout << "  ";
        }
        for (int column = 0; column < MAP_SIZE; column++) {
            cout << (*map)[row][column] << " ";
        }
        cout << "\n";
    }
}

/**
 * Creates a copy of the main map.
 *
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *      vector<vector<int>> *copy_map  - Pointer to the uninitialised copied map
 *  Returns:
 *      -
 */
void copy_of_map(vector<vector<int> > *map, vector<vector<int> > *copy_map) {
    for (int row = 0; row < MAP_SIZE; row++) {
        for (int column = 0; column < MAP_SIZE; column++) {
            (*copy_map)[row][column] = (*map)[row][column];
        }      
    } 
}

/**
 * Places asteroids on the map.
 *
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *      int mode  - Game mode
 *  Returns:
 *      -
 */
void place_blocks(vector<vector<int> > *map, int mode) {
    // Places blocks depending on the game mode.
    switch (mode) {
        case EASY:
            configure_easy(map);
            break;
        case INTERMEDIATE:
            configure_intermediate(map);
            break;
        case EXPERT:
            configure_expert(map);
            break;
        case CUSTOM:
            // Prompts the player to configure a custom map.
            configure_custom(map);
            help_message();
            break;
    }
}

/**
 * Determines and executes commands given.
 *
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *  Returns:
 *      -
 */
void process_commands(vector<vector<int> > *map) {
    // Creates a copy of the main map to be used in later functions.
    vector<vector<int> > copy_map(MAP_SIZE);
    for (int i = 0; i < MAP_SIZE; i++) {
        copy_map[i].resize(MAP_SIZE);
    }

    // Place the spacecraft in the middle of the map.
    int laserY = MAP_SIZE / 2;

    // All the main commands in the program are here.
    int command;
    int rotate_invalid = NOT_INVALID;

    // Determines and execute commands.
    while (cin >> command) { 
        switch (command) {
            case HELP:
                help_message();
                break;
            case MOVE_SPACECRAFT:
                do_move_spacecraft(&laserY);
                break;
            case FIRE_LASER:
                // The nearest block with an asteroid changes to 0.
                // Any adjacent block also gets changed to 0.
                destroy_block(map, laserY);

                // If entire map is empty, then game is won
                if (check_if_win(map, laserY)) {
                    print_map(map, laserY);
                    cout << "\n";
                    cout << "CONGRATULATIONS!\n";
                    cout << "YOU HAVE DESTROYED ALL ASTEROIDS!\n" << "\n";
                    exit(EXIT_SUCCESS);
                }
                break;
            case SHIFT_MAP:
                // Shifts the map left.
                // The game is lost if there is an asteroid at the edge of the map.
                if (check_if_lose(map, laserY)) {
                    exit(EXIT_SUCCESS);
                }
                // Shift map if game is not lost.
                shift_map(map);    
                break;
            case ROTATE:
                // Rotates the entire map either clockwise or anti-clockwise.
                // or counter-clockwise when 4 is pressed
                int rotate;
                cin >> rotate;
                if (rotate_invalid != INVALID) {    
                    // Makes a copy of the main map.
                    copy_of_map(map, &copy_map);   
                    rotate_map(map, &copy_map, rotate);  
                } 
                // Sets rotate command to invalid if a valid rotation happens.
                rotate_invalid = INVALID;    
                break;
        }

        // Prints the current state of the map.
        if (command != HELP) {
            print_map(map, laserY);     
        }
    }
}

/**
 * Destroys an asteroid. Converts value of asteroid and adjacent blocks to 0.
 * If asteroid was a TNT, calculates the blast radius and converts everything in
 * the radius to 0.
 * 
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *      int laserY  - Position of spacecraft
 *  Returns:
 *      -
 */
void destroy_block(vector<vector<int> > *map, int laserY) {
    int stop_loop = 0;
    for (int column = 0; column < MAP_SIZE && stop_loop != 1; column++) {
        if ((*map)[laserY][column] != EMPTY && stop_loop != 1) {    
            // If there is a TNT block behind, above or below an asteroid,
            // it will be triggered.
            if (TNT_MIN <= (*map)[laserY][column] && 
                (*map)[laserY][column] <= TNT_MAX && 
                stop_loop != 1) {   
                boom(map, laserY, column, stop_loop);
            } else if ((column + 1) < MAP_SIZE && 
                       TNT_MIN <= (*map)[laserY][column + 1] && 
                       (*map)[laserY][column + 1] <= TNT_MAX && 
                       stop_loop != 1) {
                boom(map, laserY, column + 1, stop_loop);
            } else if (TNT_MIN <= (*map)[laserY - 1][column] && 
                       (*map)[laserY - 1][column] <= TNT_MAX && 
                       stop_loop != 1) {
                boom(map, laserY - 1, column, stop_loop);
            } else if (TNT_MIN <= (*map)[laserY + 1][column] && 
                       (*map)[laserY + 1][column] <= TNT_MAX && 
                       stop_loop != 1) {
                boom(map, laserY + 1, column, stop_loop);
            }
            (*map)[laserY][column] = EMPTY;
            (*map)[laserY][column + 1] = EMPTY;
            (*map)[laserY - 1][column] = EMPTY;
            (*map)[laserY + 1][column] = EMPTY; 
            stop_loop = 1;        
        }           
    }                           
}

/**
 * Converts all blocks in a calculated radius to 0.
 *
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *      int laserY  - Position of spacecraft
 *      int col - Keep track of which column to convert.
 *      int stop_loop - Sentinel variable.
 *  Returns:
 *      -
 */
void boom(vector<vector<int> > *map, int laserY, int col, int stop_loop) {
    int radius = (*map)[laserY][col];  
    for (int row = 0; row < MAP_SIZE; row++) {
        for (int column = 0; column < MAP_SIZE; column++) {
            // If the distance of a block is less than the value of the TNT,
            // then the block gets destroyed.
            int distance = sqrt(pow((column - col), 2) + pow((row - laserY), 2));
            if (distance < radius) {
                (*map)[row][column] = EMPTY;
                stop_loop = 1;
            }        
        }
    }
} 

/**
 * Shifts entire map to the left by one column.
 *
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *  Returns:
 *      -
 */
void shift_map(vector<vector<int> > *map) {   
    for (int row = 0; row < MAP_SIZE; row++) {
        for (int column = 0; column < MAP_SIZE; column++) {
            // Shifts block one column to the left while maintaining input value.
            if ((*map)[row][column] != EMPTY) {
                (*map)[row][column - 1] = (*map)[row][column];
                (*map)[row][column] = 0;                
            }
        }
    }  
}

/**
 * Rotates the map clockwise or anti-clockwise.
 *
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the main map
 *      vector<vector<int>> *copy_map  - Pointer to the copy of the main map
 *      int rotate - Value given to determine direction of rotation
 *  Returns:
 *      -
 */
void rotate_map(vector<vector<int> > *map, vector<vector<int> > *copy_map, int rotate) {
    for (int row = 0; row < MAP_SIZE; row++) {
        for (int column = 0; column < MAP_SIZE; column++) {
            // Rotates the map clockwise.
            if (rotate == ROTATE_CLOCKWISE) {
                (*map)[row][column] = (*copy_map)[(MAP_SIZE - 1) - column][row];
            // Rotates the map counter clockwise.
            } else if (rotate == ROTATE_ANTI_CLOCKWISE) {
                (*map)[row][column] = (*copy_map)[column][(MAP_SIZE - 1) - row];
            }                
        }
    }
}

/**
 * Checks current game for the 'lose' condition. Game Over when there is an
 * asteroid at the left-most edge of the map.
 * 
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *      int laserY  - Position of spacecraft
 *  Returns:
 *      True  - if game over
 *      False  - if game is not over
 */
bool check_if_lose(vector<vector<int> > *map, int laserY) {
    for (int row = 0; row < MAP_SIZE; row++) {
        // Checks if an asteroid is already at the edge of the map.
        // Prints an unchanged map as well as "Game Lost!"               
        if ((*map)[row][0] != 0) {                  
            print_map(map, laserY);
            cout << "\n";
            cout << "GAME OVER! \n";
            cout << "YOU LOST. BETTER LUCK NEXT TIME!\n" << "\n";
            return true;                     
        }
    }
    return false;
}

/**
 * Checks current game for the 'win' condition. Game Won when there is no more
 * asteroids on the map.
 * 
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *      int laserY  - Position of spacecraft
 *  Returns:
 *      True  - if game over
 *      False  - if game is not over
 */
bool check_if_win(vector<vector<int> > *map, int laserY) {
    // Checks if the entire map is empty.
    bool all_empty = true;
    for (int row = 0; row < MAP_SIZE; row++) {
        for (int column = 0; column < MAP_SIZE; column++) {
            if ((*map)[row][column] != EMPTY) {
                // If the map is not empty, the game is not yet won.
                all_empty = false;  
                return all_empty;                                             
            }                               
        }
    }
    return all_empty;
}

/**
 * Prints out a welcome message to the game.
 *
 *  Arguments:
 *      -
 *  Returns:
 *      -
 */
void welcome_message() {
    cout << "===============================================================\n";
    cout << "                           ASTEROIDS                           \n";
    cout << "===============================================================\n";
    cout << "Welcome to Asteroids - the remake. In this game, your spaceship\n"
         << "is under threat of being destroyed by rogue asteroids. Your job\n"
         << "is to defend your spacecraft by destroying all asteroids.      \n";
    cout << "\n";
}

/**
 * Prints out a help message detailing the various commands in the game.
 *
 *  Arguments:
 *      -
 *  Returns:
 *      -
 */
void help_message() {
    cout << "               ============                              \n";
    cout << "               | COMMANDS |                              \n";
    cout << "               ============                              \n";
    cout << "Spacecraft   : 1 [1 = MOVE_UP] [-1 = MOVE_DOWN]          \n";
    cout << "Fire Laser   : 2                                         \n";
    cout << "Shift Map    : 3                                         \n";
    cout << "Rotating Map : 4 [1 = CLOCKWISE] [2 = ANTI-CLOCKWISE]    \n";
    cout << "Help Menu    : 5                                     \n" << "\n";
}

/**
 * Prompts player to determine the game mode. Three difficulty levels to choose
 * from – Easy, Intermediate, Expert. Also has an option to do a custom game.
 *
 *  Arguments:
 *      -
 *  Returns:
 *      int mode  - the game mode
 */
int game_mode() {
    cout << "Please select the game's difficulty:\n";
    cout << "EASY [1]         - For complete beginners\n";
    cout << "INTERMEDIATE [2] - For those looking for a bit of a challenge\n";
    cout << "EXPERT [3]       - Attempt at your own risk\n";
    cout << "CUSTOM [4]       - Have some fun building your own map.\n" << "\n";
    cout << "Enter game mode: ";

    int mode;
    while (cin >> mode) {
        if (mode < 1 || mode > 4) {
            cout << "You have entered an invalid mode. Please try again.\n";
        } else {
            break;
        }
    }
    return mode;
}

/**
 * Configures the map to EASY mode. In this mode, asteroids can only spawn after
 * the middle of the map. In addition, larger value TNT asteroids may spawn.
 * 
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *  Returns:
 *      -
 */
void configure_easy(vector<vector<int> > *map) {
    cout << "You have selected EASY MODE. Have fun!\n";
    help_message();

    // Ensures different values are generated in each game.
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        int x_pos = rand() % 15;
        int y_pos = rand() % 15;

        // No asteroids will spawn near to the leftmost-edge of the map.
        if (x_pos < (MAP_SIZE / 2)) x_pos += (MAP_SIZE / 2);
        if (y_pos < (MAP_SIZE / 2)) y_pos += (MAP_SIZE / 2);

        int value = rand() % 10;
        (*map)[x_pos][y_pos] = value;
    }
}

/**
 * Configures the map to INTERMEDIATE mode. In this mode, asteroids can spawn 
 * slightly closer to the spacecraft. In addition, TNT blocks of size 2-5 only 
 * may spawn.
 * 
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *  Returns:
 *      -
 */
void configure_intermediate(vector<vector<int> > *map) {
    cout << "You have selected INTERMEDIATE MODE. Have fun!\n";
    help_message();

    // Ensures different values are generated in each game.
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        int x_pos = rand() % 15;
        int y_pos = rand() % 15;

        // No asteroids will spawn near to the leftmost-edge of the map.
        if (x_pos < (MAP_SIZE / 2)) x_pos += (MAP_SIZE / 2) - 2;
        if (y_pos < (MAP_SIZE / 2)) y_pos += (MAP_SIZE / 2) - 2;

        int value = rand() % 6;
        (*map)[x_pos][y_pos % 15] = value;
    }
}

/**
 * Configures the map to EXPERT mode. In this mode, asteroids can spawn 
 * slightly closer to the spacecraft. In addition, TNT blocks of size 2-4 only 
 * may spawn. Game may or may not be beatable due to the random nature.
 * 
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *  Returns:
 *      -
 */
void configure_expert(vector<vector<int> > *map) {
    cout << "You have selected EXPERT MODE. Good Luck!\n";
    help_message();

    // Ensures different values are generated in each game.
    srand(time(NULL));
    for (int i = 0; i < 13; i++) {
        int x_pos = rand() % 15;
        int y_pos = rand() % 15;

        // No asteroids will spawn near to the leftmost-edge of the map.
        if (x_pos < (MAP_SIZE / 2)) x_pos += (MAP_SIZE / 2) - 4;
        if (y_pos < (MAP_SIZE / 2)) y_pos += (MAP_SIZE / 2) - 4;

        int value = rand() % 5;
        (*map)[x_pos][y_pos % 15] = value;
    }
}

/**
 * Configures the map to CUSTOM mode. In this mode, players are able to specify 
 * how many asteroids there are and where are they located on the map.
 * 
 *  Arguments:
 *      vector<vector<int>> *map  - Pointer to the map
 *  Returns:
 *      -
 */
void configure_custom(vector<vector<int> > *map) {
    cout << "             ===============                          \n";
    cout << "             | CUSTOM MODE |                          \n";
    cout << "             ===============                          \n";

    cout << "How many asteroids are there on the map? \n";
    int num_asteroids;
    cin >> num_asteroids;
    cout << "\n";
    
    cout << "Enter position of asteroids: \n";
    cout << "Format: [x-coordinate] [y-coordinate] [asteroid type]\n";

    // Assigning values to respective rows and columns.
    // If there are multiple blocks.
    int count = 0;
    while (count < num_asteroids) { 
        int row_int, column_int, value;
        cin >> row_int >> column_int >> value;
        // If block placed was valid (inside bounds), then value is printed
        // If block placed was invalid (out of bounds), then nothing happens.
        if (0 <= row_int && row_int < MAP_SIZE && 0 <= column_int && column_int < MAP_SIZE) {
            if (value <= 9) {
                (*map)[row_int][column_int] = value;
            } else {
                cout << "Invalid asteroid type. Please check your input again.\n";
                continue;
            }
        } else {
            cout << "Invalid coordinates for asteroid. Please check your input again.\n";
            continue;
        }
        count++;
    }
}

/**
 * Moves the spacecraft up or down depending on command given.
 * 
 *  Arguments:
 *      int laserY  - position of spacecraft
 *  Returns:
 *      -
 */
void do_move_spacecraft(int *laserY) {
    // Moves the spacecraft downwards.
    int up_or_down;
    cin >> up_or_down;
    if (up_or_down == MOVE_DOWN) {
        *laserY = *laserY + 1;           
        // If spacecraft is at the bottom edge, 
        // the the map prints out unchanged
        if (*laserY >= MAP_SIZE) {
            *laserY = MAP_SIZE - 1;               
        }  
    } else if (up_or_down == MOVE_UP) {
        // Moves the spacecraft upwards.
        *laserY = *laserY - 1;           
        // If the spacecraft is at the top edge, 
        // the map prints out unchanged.
        if (*laserY < 0) {
            *laserY = 0;              
        } 
    }
}
