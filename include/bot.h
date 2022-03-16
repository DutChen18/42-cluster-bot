#ifndef BOT_H
#define BOT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/time.h>

enum move_type {
	move_type_rotate,
	move_type_drop,
};

enum gravity {
	north,
	north_east,
	south_east,
	south,
	south_west,
	north_west,
};

typedef enum move_type move_type_t;
typedef enum gravity gravity_t;
typedef struct random random_t;
typedef struct alloc_node alloc_node_t;
typedef struct alloc alloc_t;
typedef struct config config_t;
typedef struct coord coord_t;
typedef struct cell cell_t;
typedef struct board board_t;
typedef struct state state_t;
typedef struct move move_t;
typedef struct tokens tokens_t;
typedef struct minimax minimax_t;
typedef struct search search_t;
typedef struct game_time game_time_t;

struct tokens {
	int8_t	a;
	int8_t	b;
};

struct random {
	uint32_t state;
};

struct game_time {
	struct timeval	start;
};

struct alloc_node {
	void *ptr;
	size_t size;
	alloc_node_t *next;
	alloc_node_t *prev;
};

struct alloc {
	alloc_node_t *node;
};

struct coord {
	int q;
	int r;
	int s;
};

struct config {
	int grid_size;
	int color_count;
	int win_length;
	int token_count;
	float timeout;
	int bot_id;
	coord_t *walls;
	size_t wall_count;
};

struct cell {
	cell_t *neighbors[6];
	coord_t coord;
};

struct board {
	cell_t *cells;
	size_t cell_count;
	cell_t **drop_cells[6];
	size_t drop_cell_count[6];
	cell_t **gravity_cells[6];
	size_t gravity_cell_count[6];
	config_t *config;
};

struct state {
	board_t *board;
	int8_t *tokens;
	int bags[2];
	int turn;
	gravity_t gravity;
	int best_player;
	int best_length;
	int best_count;
};

struct move {
	size_t drop_index;
	int8_t token;
	move_type_t type;
	gravity_t gravity;
};

struct minimax {
	int mm_depth;
	int ev_samples;
};

struct search {
	move_t move;
	bool timeout;
};

extern alloc_t alloc;
extern random_t rng;
extern game_time_t g_time;

cell_t *board_get(board_t *board, coord_t coord);
void random_new(random_t *rng);
void random_with_seed(random_t *rng, uint32_t seed);
int32_t random_next(random_t *rng);

void alloc_new(alloc_t *alloc); // delete?
void *alloc_alloc(alloc_t *alloc, size_t size);
void alloc_free(alloc_t *alloc, void *ptr);

int coord_to_pos(coord_t coord, gravity_t gravity);
coord_t coord_from_pos(int pos, gravity_t gravity, int size);
bool coord_eq(coord_t a, coord_t b);

void board_new(board_t *board, config_t *config); // delete?
void state_new(state_t *state, board_t *board);
void state_copy(state_t *state, state_t *other);
void state_delete(state_t *state);

void state_set_gravity(state_t *state, gravity_t gravity);
cell_t *state_get_empty(state_t *state, cell_t *cell);
int state_winner(state_t *state);
void state_move(state_t *state, move_t *move);
int8_t state_token(state_t *state, cell_t *cell);

move_t *move_gen(size_t *size, state_t *state, tokens_t tokens);
void search(state_t *state, move_t *move, tokens_t token);

float	get_time(void);
void	time_init(void);

#endif
