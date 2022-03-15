#include "bot.h"

void state_new(state_t *state, board_t *board)
{
	state->board = board;
	state->tokens = alloc_alloc(&alloc, sizeof(*state->tokens) * board->cell_count);
	for (size_t i = 0; i < board->cell_count; i++)
		state->tokens[i] = -1;
	state->bags[0] = board->config->token_count * board->config->color_count;
	state->bags[1] = board->config->token_count * board->config->color_count;
	state->turn = 0;
	state->gravity = south;
	state->best_player = -1;
	state->best_length = 0;
	state->best_count = 0;
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
	state->best_player = other->best_player;
	state->best_length = other->best_length;
	state->best_count = other->best_count;
}

void state_delete(state_t *state)
{
	alloc_free(&alloc, state->tokens);
}

int8_t state_token(state_t *state, cell_t *cell)
{
	return state->tokens[cell - state->board->cells];
}