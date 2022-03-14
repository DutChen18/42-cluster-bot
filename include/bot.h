#ifndef BOT_H
#define BOT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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
typedef struct alloc_node alloc_node_t;
typedef struct alloc alloc_t;
typedef struct config config_t;
typedef struct coord coord_t;
typedef struct cell cell_t;
typedef struct board board_t;
typedef struct state state_t;
typedef struct moved_state moved_state_t;
typedef struct move move_t;
typedef struct movegen movegen_t;

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
};

struct moved_state {
	state_t tmp;
	state_t *state;
	move_type_t type;
	cell_t *cell;
};

struct move {
	move_type_t type;
	int8_t token;
	size_t drop_index;
	gravity_t gravity;
};

struct movegen {
	state_t *state;
	move_t move;
	bool all_tokens;
	int8_t tokens[2];
};

extern alloc_t alloc;

void alloc_new(alloc_t *alloc); // delete?
void *alloc_alloc(alloc_t *alloc, size_t size);
void alloc_free(alloc_t *alloc, void *ptr);

int coord_to_pos(coord_t coord, gravity_t gravity);
coord_t coord_from_pos(int pos, gravity_t gravity, int size);

void board_new(board_t *board, config_t *config); // delete?
void state_new(state_t *state, board_t *board);
void state_copy(state_t *state, state_t *other);
void state_delete(state_t *state);

void state_set_gravity(state_t *state, gravity_t gravity);
cell_t *state_get_empty(state_t *state, cell_t *cell);
int state_winner(state_t *state);
void state_make_move(moved_state_t *moved_state, state_t *state, move_t *move);
void state_unmake_move(moved_state_t *moved_state);

void movegen_new(movegen_t *movegen, state_t *state, bool all_tokens);
int movegen_next(movegen_t *movegen, move_t *move);

#endif
