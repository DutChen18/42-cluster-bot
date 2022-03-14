#ifndef BOT_H
#define BOT_H

#include <stdint.h>
#include <stddef.h>

enum move_type {
	move_type_rotate,
	move_type_drop,
};

typedef enum move_type move_type_t;
typedef struct alloc_node alloc_node_t;
typedef struct alloc alloc_t;
typedef struct config config_t;
typedef struct coord coord_t;
typedef struct cell cell_t;
typedef struct board board_t;
typedef struct state state_t;
typedef struct moved_state moved_state_t;
typedef struct move move_t;

struct alloc_node {
	void *ptr;
	size_t size;
	alloc_node_t *next;
	alloc_node_t *prev;
};

struct alloc {
	alloc_node_t *node;
};

struct config {
	int grid_size;
	int color_count;
	int win_length;
};

struct coord {
	int q;
	int r;
	int s;
};

struct cell {
	cell_t *neighbors[6];
	coord_t coord;
};

struct board {
	cell_t *cells;
	size_t cell_count;
	size_t *drop_cells;
	size_t drop_cell_count;
	size_t *gravity_cells;
	size_t gravity_cell_count;
	config_t *config;
};

struct state {
	board_t *board;
	int8_t *tokens;
	int bags[2];
	int turn;
};

struct moved_state {
	state_t tmp;
	state_t *state;
	move_type_t type;
	size_t cell_index;
};

struct move {
	move_type_t type;
	int8_t token;
	int pos;
	int gravity;
};

extern alloc_t alloc;

void alloc_new(alloc_t *alloc); // delete?
void *alloc_alloc(alloc_t *alloc, size_t size);
void alloc_free(alloc_t *alloc, void *ptr);

void board_new(board_t *board, config_t *config); // delete?
void state_new(state_t *state, board_t *board);
void state_copy(state_t *state, state_t *other);
void state_delete(state_t *state);

void state_move_drop(state_t *state, int pos, int8_t token);
void state_move_gravity(state_t *state, int gravity);
void state_make_move(moved_state_t *moved_state, state_t *state, move_t *move);
void state_unmake_move(moved_state_t *moved_state);

#endif
