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

static void check_consistency(state_t *state)
{
	int gravity;
	printf("fetch gravity\n");
	scanf(" gravity %d", &gravity);
	assert((int) state->gravity == gravity);
	printf("fetch cells\n");
	int cell_count;
	scanf(" cell_count %d", &cell_count);
	for (int i = 0; i < cell_count; i++) {
		int q, r, s, value;
		scanf(" cell %d %d %d %d", &q, &r, &s, &value);
		cell_t *cell = board_get(state->board, q, r, s);
		(void) cell;
		assert(cell != NULL);
		assert(state->tokens[cell - state->board->cells] == value);
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
	int			wall_count;

	alloc_new(&alloc);
	random_new(&rng);
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
	config.color_count /= 2;
	printf("fetch walls\n");
	scanf(" wall_count %d", &wall_count);
	config.wall_count = wall_count;
	config.walls = malloc(sizeof(*config.walls) * config.wall_count);
	for (size_t i = 0; i < config.wall_count; i++) {
		int q, r, s;
		scanf(" wall %d %d %d", &q, &r, &s);
		config.walls[i] = (coord_t) { .q = q, .r = r, .s = s };
	}
	board_new(&board, &config);
	state_new(&state, &board);

	printf("color %d\n", 5);
	if (config.bot_id == 1) {
		read_action(&move, &state);
		state_move(&state, &move);
	}
	
	while(1) {
		scanf(" chips %d %d", &token_a, &token_b);
		check_consistency(&state);
		search(&state, &move, token_a, token_b);
		write_action(&move, &state);
		state_move(&state, &move);
		read_action(&move, &state);
		state_move(&state, &move);
	}
	return EXIT_SUCCESS;
}
