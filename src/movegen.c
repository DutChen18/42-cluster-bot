#include "bot.h"

static int movegen_next_pseudo(movegen_t *movegen, move_t *move)
{
	switch (movegen->move.type) {
	case move_type_rotate:
		if (movegen->move.gravity < 6) {
			*move = movegen->move;
			movegen->move.gravity += 1;
			return 1;
		}
		movegen->move.type = move_type_drop;
		movegen->move.drop_index = 0;
		movegen->move.token = 0;
	case move_type_drop:
	drop_start:
		if (movegen->move.drop_index < movegen->state->board->drop_cell_count[movegen->state->gravity]) {
			*move = movegen->move;
			movegen->move.drop_index += 1;
			return 1;
		}
		movegen->move.drop_index = 0;
		movegen->move.token += 1;
		if (movegen->move.token < movegen->state->board->config->color_count * 2)
			goto drop_start;
		return 0;
	}
}

static int movegen_legal(movegen_t *movegen, move_t *move)
{
	switch (move->type) {
	case move_type_rotate:
		return movegen->state->bags[movegen->state->turn] >= 2;
	case move_type_drop:
		if (movegen->all_tokens && move->token < movegen->state->turn * movegen->state->board->config->color_count)
			return 0;
		if (movegen->all_tokens && move->token >= (movegen->state->turn + 1) * movegen->state->board->config->color_count)
			return 0;
		if (!movegen->all_tokens && move->token != movegen->tokens[0] && move->token != movegen->tokens[1])
			return 0;
		if (movegen->state->tokens[movegen->state->board->drop_cells[movegen->state->gravity][move->drop_index] - movegen->state->board->cells] != -1)
			return 0;
		return movegen->state->bags[movegen->state->turn] >= 1;
	}
}

void movegen_new(movegen_t *movegen, state_t *state, bool all_tokens)
{
	movegen->state = state;
	movegen->all_tokens = all_tokens;
	movegen->move.type = move_type_rotate;
	movegen->move.gravity = north;
}

int movegen_next(movegen_t *movegen, move_t *move)
{
	while (1) {
		if (!movegen_next_pseudo(movegen, move))
			return 0;
		if (movegen_legal(movegen, move))
			return 1;
	}
}
