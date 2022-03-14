#include "bot.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	alloc_new(&alloc);
	config_t config;
	config.grid_size = 4;
	config.color_count = 2;
	config.win_length = 0;
	config.wall_count = 0;
	config.token_count = 10;
	board_t board;
	state_t state;
	board_new(&board, &config);
	state_new(&state, &board);
	size_t size;
	move_t *moves = move_gen(&size, &state, true, -1, -1);
	printf("%d\n", (int) board.drop_cell_count[south]);
	for (size_t i = 0; i < size; i++)
		printf("%d\n", moves[i].type);
	return EXIT_SUCCESS;
}
