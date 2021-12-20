#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "asteroids.hpp"

// Function to print out the contents of the main map array. 
// Also print out a ">" symbol to denote the current laser position.
void print_map(int map[MAP_SIZE][MAP_SIZE], int laserY) {
    int i = 0;
    while (i < MAP_SIZE) {
        int j = 0;
        if (i == laserY) {      
            std::cout << "> ";
        } else {
            std::cout << "  ";
        }
        while (j < MAP_SIZE) {
            std::cout << map[i][j] << " ";
            j++;
        }
        std::cout << "\n";
        i++;
    }
}

// Function to create a copy of the main map.....................................
void copy_of_map(int map[MAP_SIZE][MAP_SIZE], int copy_map[MAP_SIZE][MAP_SIZE]) {
    int i = 0;
    while (i < MAP_SIZE) {
        int j = 0;
        while (j < MAP_SIZE) {
            copy_map[i][j] = map[i][j];
            j++;
        }
        i++;           
    } 
}

// Function to place blocks on the main map......................................
void place_blocks(int map[MAP_SIZE][MAP_SIZE]) {
    int num_of_blocks;
    std::cout << "How many blocks? ";
    std::cin >> num_of_blocks;
    
    // The following lines prompts the player to enter 
    // the position and amount of block in the map and prints the map.
    int row_int, column_int, value; 
    std::cout << "Enter blocks:\n";

    // Placing blocks on the map.
    if (num_of_blocks > 1) {      
        // Assigning values to respective rows and columns.
        // If there are multiple blocks.
        int f = 0;
        while (f < num_of_blocks) { 
            std::cin >> row_int >> column_int >> value;
            // If block placed was valid (inside bounds), then value is printed
            // If block placed was invalid (out of bounds), then nothing happens
            if (0 <= row_int && row_int < MAP_SIZE && 0 <= column_int && column_int < MAP_SIZE) {
                map[row_int][column_int] = value;
            }                                          
            f++;
        }
        // If there is only one block.              
    } else if (num_of_blocks == 1) {
        std::cin >> row_int >> column_int >> value;
        map[row_int][column_int] = value; 
    }
}

// Function that holds all of the main commands in the program...................
void process_commands(int map[MAP_SIZE][MAP_SIZE]) {
    // Creates a copy of the main map to be used in later functions.
    int copy_map[MAP_SIZE][MAP_SIZE] = {EMPTY};
    // This line creates out laserY variable. The laser starts in the
    // middle of the map, at position 7.
    int laserY = MAP_SIZE / 2;

    // All the main commands in the program are here.
    int static_num, rotate, up_or_down;
    int rotate_invalid = NOT_INVALID;
    int e = 0;
    // The following lines moves the laser upwards or downwards.....................
    // While the inputs are valid and not EOF, move the laser up or down
    while (std::cin >> static_num) { 
        // If player inputs 1 1, then laser moves downwards     
        if (static_num == MOVE_LASER) {
            std::cin >> up_or_down;
            if (up_or_down == 1) {
                laserY = laserY + 1;           
                // If laser is at the bottom edge, the the map prints out unchanged
                if (laserY >= MAP_SIZE) {
                    laserY = MAP_SIZE - 1;               
                }  
                // If player inputs 1 -1, then laser moves upwards  
            } else if (up_or_down == -1) {
                laserY = laserY - 1;           
                // If the laser is at the top edge, the map prints out unchanged
                if (laserY < 0) {
                    laserY = 0;              
                } 
            }

            // The following lines fires the laser when the player inputs 2.................
            // When 2 is entered, the nearest block with a value of 1 changes to 0
            // Any adjacent block gets changed to 0 too 
        } else if (static_num == FIRE_LASER) {
            destroy_block(map, laserY);
            int all_empty = check_if_win(map, laserY);

            // If entire map is empty, then game is won
            if (all_empty == 1) {
                print_map(map, laserY);
                std::cout << "Game Won!\n";
                exit(0);
            }
            
            // STAGE 2: Shifting the map to the left
            // The following lines shift the map to the left when 3 is pressed,
            // If there is a stone at the edge of the map then the game is lost ............                                                           
        } else if (static_num == SHIFT_MAP) {            
            if (check_if_lose(map, laserY) == 1) {
                exit(0);
            }
            // If game is not lost, the entire map shifts to the left  
            shift_map(map);    

            // STAGE 3: Rotate The Map
            // The following lines rotates the enitre map either clockwise
            // or counter-clockwise when 4 is pressed.......................................
        } else if (static_num == ROTATE) {
            std::cin >> rotate;
            
            if (rotate_invalid != INVALID) {    
                // Makes a copy of the main map and saves it to another array
                copy_of_map(map, copy_map);

                // The following lines rotate the map clockwise if 1 is pressed or
                // rotates the map counter-clockwise if 2 is pressed........................               
                rotate_map(map, copy_map, rotate);
                
            } 
            // Sets rotate command to invalid as soon as a valid rotation happens
            rotate_invalid = INVALID;            
        }

        // If anything else is entered, then nothing happens and map prints unchanged
        print_map(map, laserY);     
        e++;
    }
}


void destroy_block(int map[MAP_SIZE][MAP_SIZE], int laserY) {
    // A sentinel variable to ensure that the loop stops
    // When a 2 is pressed / block is destroyed
    int stop_loop = 0;
    int g = 0;  
    while (g < MAP_SIZE && stop_loop != 1) {
        if (map[laserY][g] != EMPTY && stop_loop != 1) {    
            // STAGE 3: Destroy blocks with TNT
            // If there is a TNT block behind, above or below a STONE block,
            // it will be triggered
            if (TNT_MIN <= map[laserY][g] && map[laserY][g] <= TNT_MAX && 
                stop_loop != 1) {   
                boom(map, laserY, g, stop_loop);
            } else if ((g + 1) < MAP_SIZE && 
                       TNT_MIN <= map[laserY][g + 1] && map[laserY][g + 1] <= TNT_MAX && 
                       stop_loop != 1) {
                boom(map, laserY, g + 1, stop_loop);
            } else if (TNT_MIN <= map[laserY - 1][g] && map[laserY - 1][g] <= TNT_MAX && 
                       stop_loop != 1) {
                boom(map, laserY - 1, g, stop_loop);
            } else if (TNT_MIN <= map[laserY + 1][g] && map[laserY + 1][g] <= TNT_MAX && 
                       stop_loop != 1) {
                boom(map, laserY + 1, g, stop_loop);
            }
            map[laserY][g] = EMPTY;
            map[laserY][g + 1] = EMPTY;
            map[laserY - 1][g] = EMPTY;
            map[laserY + 1][g] = EMPTY; 
            stop_loop = 1;        
        }         
        g++; 
    }                           
}

void boom(int map[MAP_SIZE][MAP_SIZE], int laserY, int g, int stop_loop) {
    int radius = map[laserY][g];  
    int h = 0;
    while (h < MAP_SIZE) {
        int i = 0;
        while (i < MAP_SIZE) {
            // If the distance of a block is less than the value of the TNT,
            // then the block gets destroyed
            int distance = sqrt(pow((i - g), 2) + pow((h - laserY), 2));
            if (distance < radius) {
                map[h][i] = EMPTY;
                stop_loop = 1;
            } 
            i++;            
        }
        h++;
    }
} 

// Function that shifts entire map to the left..................................
void shift_map(int map[MAP_SIZE][MAP_SIZE]) {
    int l = 0;     
    while (l < MAP_SIZE) {
        int m = 0; 
        while (m < MAP_SIZE) {
            // Checks if a block in array is equal to the input value
            // If yes, then shifts it 1 column to the left
            if (map[l][m] != EMPTY) {
                map[l][m - 1] = map[l][m];
                map[l][m] = 0;                
            }
            m++;
        }
        l++;
    }  
}

// Function that rotates the map clockwise or counter-clockwise..................
void rotate_map(int map[MAP_SIZE][MAP_SIZE], int copy_map[MAP_SIZE][MAP_SIZE], int rotate) {
    int j = 0;
    while (j < MAP_SIZE) {
        int k = 0;
        while (k < MAP_SIZE) {
            // If rotate = 1, then rotates the map clockwise
            if (rotate == 1) {
                map[j][k] = copy_map[(MAP_SIZE - 1) - k][j];
            // If rotate = 2, then rotates the map counter clockwise
            } else if (rotate == 2) {
                map[j][k] = copy_map[k][(MAP_SIZE - 1) - j];
            }                
            k++;  
        }
        j++;
    }
}

// Function to check the "lose condition".......................................
int check_if_lose(int map[MAP_SIZE][MAP_SIZE], int laserY) {
    // Stage 2:End game condition (Loss) when there is a stone at edge of the map            
    int n = 0;
    while (n < MAP_SIZE) {
        // When 3 is pressed, checks if a stone is already at the edge of the map
        // prints an unchanged map as well as "Game Lost!"               
        if (map[n][0] != 0) {                  
            print_map(map, laserY);  
            std::cout << "Game Lost!\n";
            return 1;                                                                                  
        }   
        n++;
    }
    return 0;
}

// Function to check the "win condition"........................................
int check_if_win(int map[MAP_SIZE][MAP_SIZE], int laserY) {
    // Stage 2: End game condition (Win) when all stones are destroyed
    // A variable to check if the enitire map is empty
    int all_empty = 1;
    int o = 0;
    while (o < MAP_SIZE) {
        int p = 0;
        while (p < MAP_SIZE) {
            if (map[o][p] != EMPTY) {
                // If the map is not empty, then the variable becomes false
                all_empty = 0;  
                return all_empty;                                             
            }                               
            p++;
        }
        o++;
    }
    // If the map is empty, then the variable is true
    return all_empty;
}