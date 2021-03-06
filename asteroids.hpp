/*=============================================================================

        Header file that contains constants and function definitions.

==============================================================================*/

#include <vector>
using namespace std;

// Size of map.
#define MAP_SIZE 15

// Values in the main map.
#define EMPTY 0
#define ASTEROID 1
#define TNT_MIN 2
#define TNT_MAX 9

// Game modes.
#define EASY 1
#define INTERMEDIATE 2
#define EXPERT 3
#define CUSTOM 4

// Command values.
#define HELP 5
#define MOVE_SPACECRAFT 1
#define MOVE_UP -1
#define MOVE_DOWN 1
#define FIRE_LASER 2
#define SHIFT_MAP 3
#define ROTATE 4
#define ROTATE_CLOCKWISE 1
#define ROTATE_ANTI_CLOCKWISE 2
#define NOT_INVALID 0
#define INVALID 1

int game_mode();
void help_message();
void do_move_spacecraft(int *laserY);
void configure_easy(vector<vector<int> > *map);
void configure_intermediate(vector<vector<int> > *map);
void configure_expert(vector<vector<int> > *map);
void configure_custom(vector<vector<int> > *map);
void welcome_message();
void shift_map(vector<vector<int> > *map);
void process_commands(vector<vector<int> > *map);
void print_map(vector<vector<int> > *map, int laserY);
void place_blocks(vector<vector<int> > *map, int mode);
bool check_if_win(vector<vector<int> > *map, int laserY);
bool check_if_lose(vector<vector<int> > *map, int laserY);
void destroy_block(vector<vector<int> > *map, int laserY);
void boom(vector<vector<int> > *map, int laserY, int col, int stop_loop);
void copy_of_map(vector<vector<int> > *map, vector<vector<int> > *copy_map);
void rotate_map(vector<vector<int> > *map, vector<vector<int> > *copy_map, int rotate);

