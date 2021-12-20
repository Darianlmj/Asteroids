#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include "asteroids.hpp"

using namespace std;

// Function to print out the contents of the main map array. 
// Also print out a ">" symbol to denote the current laser position.
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

// Function to create a copy of the main map.....................................
void copy_of_map(vector<vector<int> > *map, vector<vector<int> > *copy_map) {
    for (int row = 0; row < MAP_SIZE; row++) {
        for (int column = 0; column < MAP_SIZE; column++) {
            (*copy_map)[row][column] = (*map)[row][column];
        }      
    } 
}

// Function to place blocks on the main map......................................
void place_blocks(vector<vector<int> > *map) {
    // Prompts the player to enter the amount and position of blocks in the map.
    int num_asteroids;
    cout << "How many asteroids are there on the map? \n";
    cin >> num_asteroids;
    cout << "\n";
    
    int row_int, column_int, value;
    cout << "Enter position of asteroids: \n";
    cout << "Format: [x-coordinate] [y-coordinate] [asteroid type]\n";

    // Assigning values to respective rows and columns.
    // If there are multiple blocks.
    int count = 0;
    while (count < num_asteroids) { 
        cin >> row_int >> column_int >> value;
        // If block placed was valid (inside bounds), then value is printed
        // If block placed was invalid (out of bounds), then nothing happens
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

// Function that holds all of the main commands in the program...................
void process_commands(vector<vector<int> > *map) {
    // Creates a copy of the main map to be used in later functions.
    vector<vector<int> > copy_map(MAP_SIZE);
    for (int i = 0; i < MAP_SIZE; i++) {
        copy_map[i].resize(MAP_SIZE);
    }

    // This line creates out laserY variable. The laser starts in the
    // middle of the map, at position 7.
    int laserY = MAP_SIZE / 2;

    // All the main commands in the program are here.
    int command, rotate, up_or_down, all_empty;
    int rotate_invalid = NOT_INVALID;

    // The following lines moves the laser upwards or downwards.....................
    // While the inputs are valid and not EOF, move the laser up or down
    while (cin >> command) { 
        switch (command) {
            case HELP:
                help_message();
                break;
            case MOVE_LASER:
                // If player inputs 1 1, then laser moves downwards 
                cin >> up_or_down;
                if (up_or_down == MOVE_DOWN) {
                    laserY = laserY + 1;           
                    // If laser is at the bottom edge, the the map prints out unchanged
                    if (laserY >= MAP_SIZE) {
                        laserY = MAP_SIZE - 1;               
                    }  
                    // If player inputs 1 -1, then laser moves upwards  
                } else if (up_or_down == MOVE_UP) {
                    laserY = laserY - 1;           
                    // If the laser is at the top edge, the map prints out unchanged
                    if (laserY < 0) {
                        laserY = 0;              
                    } 
                }
                break;
            case FIRE_LASER:
                // When 2 is entered, the nearest block with a value of 1 changes to 0
                // Any adjacent block gets changed to 0 too 
                destroy_block(map, laserY);
                all_empty = check_if_win(map, laserY);

                // If entire map is empty, then game is won
                if (all_empty == 1) {
                    print_map(map, laserY);
                    cout << "Game Won!\n";
                    exit(0);
                }
                break;
            case SHIFT_MAP:
                // The following lines shift the map to the left when 3 is pressed,
                // If there is a stone at the edge of the map then the game is lost
                if (check_if_lose(map, laserY) == 1) {
                    exit(0);
                }
                // If game is not lost, the entire map shifts to the left  
                shift_map(map);    
                break;
            case ROTATE:
                // The following lines rotates the entire map either clockwise
                // or counter-clockwise when 4 is pressed
                cin >> rotate;
                if (rotate_invalid != INVALID) {    
                    // Makes a copy of the main map and saves it to another array
                    copy_of_map(map, &copy_map);

                    // The following lines rotate the map clockwise if 1 is pressed or
                    // rotates the map counter-clockwise if 2 is pressed........................               
                    rotate_map(map, &copy_map, rotate);  
                } 
                // Sets rotate command to invalid as soon as a valid rotation happens
                rotate_invalid = INVALID;    
                break;
        }
        if (command != 5) {
            // If anything else is entered, then nothing happens and map prints unchanged
            print_map(map, laserY);     
        }
    }
}

void destroy_block(vector<vector<int> > *map, int laserY) {
    // When a 2 is pressed / block is destroyed
    int stop_loop = 0;
    int column = 0;  
    while (column < MAP_SIZE && stop_loop != 1) {
        if ((*map)[laserY][column] != EMPTY && stop_loop != 1) {    
            // STAGE 3: Destroy blocks with TNT
            // If there is a TNT block behind, above or below a STONE block,
            // it will be triggered
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
        column++; 
    }                           
}

void boom(vector<vector<int> > *map, int laserY, int column, int stop_loop) {
    int radius = (*map)[laserY][column];  
    for (int row = 0; row < MAP_SIZE; row++) {
        for (int column = 0; column < MAP_SIZE; column++) {
            // If the distance of a block is less than the value of the TNT,
            // then the block gets destroyed
            int distance = sqrt(pow((column - column), 2) + pow((row - laserY), 2));
            if (distance < radius) {
                (*map)[row][column] = EMPTY;
                stop_loop = 1;
            }        
        }
    }
} 

// Function that shifts entire map to the left..................................
void shift_map(vector<vector<int> > *map) {   
    for (int row = 0; row < MAP_SIZE; row++) {
        for (int column = 0; column < MAP_SIZE; column++) {
            // Checks if a block in array is equal to the input value
            // If yes, then shifts it 1 column to the left
            if ((*map)[row][column] != EMPTY) {
                (*map)[row][column - 1] = (*map)[row][column];
                (*map)[row][column] = 0;                
            }
        }
    }  
}

// Function that rotates the map clockwise or counter-clockwise..................
void rotate_map(vector<vector<int> > *map, vector<vector<int> > *copy_map, int rotate) {
    for (int row = 0; row < MAP_SIZE; row++) {
        for (int column = 0; column < MAP_SIZE; column++) {
            // Rotates the map clockwise
            if (rotate == ROTATE_CLOCKWISE) {
                (*map)[row][column] = (*copy_map)[(MAP_SIZE - 1) - column][row];
            // Rotates the map counter clockwise
            } else if (rotate == ROTATE_ANTI_CLOCKWISE) {
                (*map)[row][column] = (*copy_map)[column][(MAP_SIZE - 1) - row];
            }                
        }
    }
}

// Function to check the "lose condition".......................................
int check_if_lose(vector<vector<int> > *map, int laserY) {
    // Stage 2:End game condition (Loss) when there is a stone at edge of the map            
    for (int row = 0; row < MAP_SIZE; row++) {
        // When 3 is pressed, checks if a stone is already at the edge of the map
        // prints an unchanged map as well as "Game Lost!"               
        if ((*map)[row][0] != 0) {                  
            print_map(map, laserY);  
            cout << "Game Lost!\n";
            return 1;                                                                                  
        }   
    }
    return 0;
}

// Function to check the "win condition"........................................
int check_if_win(vector<vector<int> > *map, int laserY) {
    // Stage 2: End game condition (Win) when all stones are destroyed
    // A variable to check if the enitire map is empty
    int all_empty = 1;
    for (int row = 0; row < MAP_SIZE; row++) {
        for (int column = 0; column < MAP_SIZE; column++) {
            if ((*map)[row][column] != EMPTY) {
                // If the map is not empty, then the variable becomes false
                all_empty = 0;  
                return all_empty;                                             
            }                               
        }
    }
    // If the map is empty, then the variable is true
    return all_empty;
}

void welcome_message() {
    cout << "===============================================================\n";
    cout << "                           ASTEROIDS                           \n";
    cout << "===============================================================\n";
    cout << "Welcome to Asteroids - the remake. In this game, your spaceship\n"
         << "is under threat of being destroyed by rogue asteroids. Your job\n"
         << "is to defend your spacecraft by destroying all asteroids.    \n\n";
    help_message();
    cout << "\n";
}

void help_message() {
    cout << "               ============                          \n";
    cout << "               | COMMANDS |                          \n";
    cout << "               ============                          \n";
    cout << "Spacecraft   : 1 [1=MOVE_UP] [-1=MOVE_DOWN]          \n";
    cout << "Fire Laser   : 2                                     \n";
    cout << "Shift Map    : 3                                     \n";
    cout << "Rotating Map : 4 [1=CLOCKWISE] [2=ANTI-CLOCKWISE]    \n";
    cout << "Help Menu    : 5                                     \n";
}