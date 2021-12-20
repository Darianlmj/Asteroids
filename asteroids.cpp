#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include "asteroids.hpp"

using namespace std;

int main() {
    vector<vector<int> > map(MAP_SIZE);

    for (int i = 0; i < MAP_SIZE; i++) {
        map[i].resize(MAP_SIZE);
    }
    // int map[MAP_SIZE][MAP_SIZE] = {EMPTY};

    place_blocks(&map);
    print_map(&map, MAP_SIZE / 2);

    process_commands(&map);
}


