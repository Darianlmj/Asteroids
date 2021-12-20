#include <iostream>
#include <vector>
#include "asteroids.hpp"

using namespace std;

int main() {
    // Create the map.
    vector<vector<int> > map(MAP_SIZE);
    for (int i = 0; i < MAP_SIZE; i++) {
        map[i].resize(MAP_SIZE);
    }

    // Place blocks on the map.
    place_blocks(&map);
    // Printing the current state of the map.
    print_map(&map, MAP_SIZE / 2);

    // Process any commands.
    process_commands(&map);
}


