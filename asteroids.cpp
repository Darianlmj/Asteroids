#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "asteroids.hpp"

int main() {
    int map[MAP_SIZE][MAP_SIZE] = {EMPTY};

    place_blocks(map);
    print_map(map, MAP_SIZE / 2);

    process_commands(map);
}


