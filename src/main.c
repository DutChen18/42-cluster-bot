#include "bot.h"
#include <stdio.h>
#include <stdlib.h>

int	get_index_from_coord(coord_t coord, state_t *state)
{
	int	i;

	i = 0;
	while (i < 10)
	{
		state->board->drop_cells[i];
		printf();
		i++;
	}
}

static void	read_action(move_t *move, state_t *state)
{
	char	action[256];
	int		gravity;
	int		token;
	int		drop;
	coord_t	coord;

	scanf("%10s", action);
	if (strcmp("drop", action) == 0) {
		scanf("%d %d", &drop, &token);
		move->token = token;
		coord = coord_from_pos(drop, state->gravity, state->board->config->grid_size);
		move->drop_index = get_index_from_coord(coord, state);
		move->type = move_type_drop;
	} else if (strcmp("rotate", action) == 0) {
		scanf("%d", &gravity);
		move->gravity = gravity;
		move->type = move_type_rotate;
	}
}

int main(void)
{
	config_t	config;
	move_t		move;
	state_t		state;

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
	printf("color %d\n", 5);
	if (config.bot_id == 1) {
		read_action(&move, &state);
	while(1)
	{
		scanf();
	}
	printf("Hello, World!\n");
	return EXIT_SUCCESS;
}
