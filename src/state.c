#include "bot.h"

void state_new(state_t *state, board_t *board)
{
	state->board = board;
	state->tokens = alloc_alloc(&alloc, sizeof(*state->tokens) * board->cell_count);
	state->bags[0] = board->config->token_count * board->config->color_count;
	state->bags[1] = board->config->token_count * board->config->color_count;
	state->turn = 0;
	state->gravity = south;
}

void state_copy(state_t *state, state_t *other)
{
	state->board = other->board;
	state->tokens = alloc_alloc(&alloc, sizeof(*state->tokens) * other->board->cell_count);
	for (size_t i = 0; i < other->board->cell_count; i++)
		state->tokens[i] = other->tokens[i];
	state->bags[0] = other->bags[0];
	state->bags[1] = other->bags[1];
	state->turn = other->turn;
	state->gravity = other->gravity;
}

void state_delete(state_t *state)
{
	alloc_free(&alloc, state->tokens);
}
