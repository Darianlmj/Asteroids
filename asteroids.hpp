#include <vector>

using namespace std;

// Size of map.
#define MAP_SIZE 15

// Values inside the main map
#define EMPTY 0
#define STONE 1
#define TNT_MIN 2
#define TNT_MAX 9

//Values for certain commands
#define MOVE_LASER 1
#define FIRE_LASER 2
#define SHIFT_MAP 3
#define ROTATE 4
#define NOT_INVALID 0
#define INVALID 1

void print_map(vector<vector<int> > *map, int laserY);
void copy_of_map(vector<vector<int> > *map, vector<vector<int> > *copy_map);
void place_blocks(vector<vector<int> > *map);
void process_commands(vector<vector<int> > *map);
void boom(vector<vector<int> > *map, int laserY, int g, int stop_loop);
void rotate_map(vector<vector<int> > *map, vector<vector<int> > *copy_map, int rotate);
int check_if_lose(vector<vector<int> > *map, int laserY);
int check_if_win(vector<vector<int> > *map, int laserY);
void destroy_block(vector<vector<int> > *map, int laserY);
void shift_map(vector<vector<int> > *map);

