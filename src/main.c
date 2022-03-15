#include "bot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static size_t drop_index_from_coord(coord_t coord, state_t *state)
{
	for (size_t i = 0; i < state->board->drop_cell_count[state->gravity]; i++) {
		cell_t *cell = state->board->drop_cells[state->gravity][i];
		if (cell->coord.q == coord.q && cell->coord.r == coord.r && cell->coord.s == coord.s)
			return i;
	}
	assert(false);
	return 0;
}

static void read_action(move_t *move, state_t *state)
{
	char	action[256];
	int		gravity;
	int		token;
	int		drop;

	scanf("%255s", action);
	if (strcmp("drop", action) == 0) {
		scanf("%d %d", &drop, &token);
		move->token = token;
		coord_t coord = coord_from_pos(drop, state->gravity, state->board->config->grid_size);
		move->drop_index = drop_index_from_coord(coord, state);
		move->type = move_type_drop;
	} else if (strcmp("rotate", action) == 0) {
		scanf("%d", &gravity);
		move->gravity = gravity;
		move->type = move_type_rotate;
	}
}

static void write_action(move_t *move, state_t *state)
{
	if (move->type == move_type_drop) {
		int pos = coord_to_pos(state->board->drop_cells[state->gravity][move->drop_index]->coord, state->gravity);
		printf("drop %d %d\n", pos, (int) move->token);
	} else if (move->type == move_type_rotate) {
		printf("rotate %d\n", (int) move->gravity);
	}
}

int main(void)
{
	config_t	config;
	move_t		move;
	board_t		board;
	state_t		state;
	int			token_a;
	int			token_b;

	alloc_new(&alloc);
	setbuf(stdout, NULL);
	setbuf(stdin, NULL);
	setbuf(stderr, NULL);

	scanf(" init %d %d %d %d %f %d",
			&config.color_count,
			&config.token_count,
			&config.grid_size,
			&config.win_length,
			&config.timeout,
			&config.bot_id);
	// TODO: we could fetch walls here
	config.color_count /= 2;
	config.walls = NULL;
	config.wall_count = 0;
	board_new(&board, &config);
	state_new(&state, &board);

	printf("color %d\n", 5);
	if (config.bot_id == 1) {
		read_action(&move, &state);
		state_move(&state, &move);
	}
	
	while(1) {
		scanf(" chips %d %d", &token_a, &token_b);
		search(&state, &move, token_a, token_b);
		write_action(&move, &state);
		state_move(&state, &move);
		read_action(&move, &state);
		state_move(&state, &move);
	}
	return EXIT_SUCCESS;
}
