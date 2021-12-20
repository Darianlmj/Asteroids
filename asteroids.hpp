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

void print_map(int map[MAP_SIZE][MAP_SIZE], int laserY);
void copy_of_map(int map[MAP_SIZE][MAP_SIZE], int copy_map[MAP_SIZE][MAP_SIZE]);
void place_blocks(int map[MAP_SIZE][MAP_SIZE]);
void process_commands(int map[MAP_SIZE][MAP_SIZE]);
void boom(int map[MAP_SIZE][MAP_SIZE], int laserY, int g, int stop_loop);
void rotate_map(int map[MAP_SIZE][MAP_SIZE], int copy_map[MAP_SIZE][MAP_SIZE], int rotate);
int check_if_lose(int map[MAP_SIZE][MAP_SIZE], int laserY);
int check_if_win(int map[MAP_SIZE][MAP_SIZE], int laserY);
void destroy_block(int map[MAP_SIZE][MAP_SIZE], int laserY);
void shift_map(int map[MAP_SIZE][MAP_SIZE]);

