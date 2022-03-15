#include "bot.h"

static int state_length(state_t *state, cell_t *cell, gravity_t gravity)
{
	int length = 0;
	cell_t *next = cell;
	while (next->neighbors[gravity] != NULL && state_token(state, next->neighbors[gravity]) == state_token(state, cell)) {
		next = next->neighbors[gravity];
		length++;
	}
	return length;
}

static void state_update(state_t *state, int length, int8_t token)
{
	int player = token / state->board->config->color_count;
	if (length > state->best_length) {
		state->best_length = length;
		state->best_player = player;
		state->best_count = 1;
	} else if (length == state->best_length && player != state->best_player) {
		state->best_count += 1;
	}
}

static void state_update_at(state_t *state, cell_t *cell)
{
	for (int i = 0; i < 3; i++) {
		if (state_token(state, cell) != -1) {
			int length = 1;
			length += state_length(state, cell, i);
			length += state_length(state, cell, i + 3);
			state_update(state, length, state_token(state, cell));
		}
	}
}

static void state_update_all(state_t *state)
{
	state->best_player = -1;
	state->best_length = 0;
	state->best_count = 0;
	for (int i = 0; i < 3; i++) {
		for (size_t j = 0; j < state->board->cell_count; j++) {
			if (state->tokens[j] != -1) {
				int length = 1;
				length += state_length(state, &state->board->cells[j], i);
				state_update(state, length, state->tokens[j]);
			}
		}
	}
}

void state_set_gravity(state_t *state, gravity_t gravity)
{
	state->gravity = gravity;
	for (size_t i = 0; i < state->board->gravity_cell_count[gravity]; i++) {
		cell_t *top = state->board->gravity_cells[gravity][i], *bot = top;
		while (top != NULL) {
			if (state_token(state, top) != -1) {
				int8_t tmp = state_token(state, bot);
				state->tokens[bot - state->board->cells] = state_token(state, top);
				state->tokens[top - state->board->cells] = tmp;
				bot = bot->neighbors[(gravity + 3) % 6];
			}
			top = top->neighbors[(gravity + 3) % 6];
		}
	}
	state_update_all(state);
}

cell_t *state_get_empty(state_t *state, cell_t *cell)
{
	while (cell->neighbors[state->gravity] != NULL && state_token(state, cell->neighbors[state->gravity]) == -1)
		cell = cell->neighbors[state->gravity];
	return cell;
}

int state_winner(state_t *state)
{
	if (state->best_player != -1 && state->best_length >= state->board->config->win_length && state->best_count == 1)
		return state->best_player;
	if (state->bags[0] == 0)
		return 1;
	if (state->bags[1] == 0)
		return 0;
	return -1;
}

void state_move(state_t *state, move_t *move)
{
	cell_t *cell;
	switch (move->type) {
	case move_type_rotate:
		state->bags[state->turn] -= 2;
		state_set_gravity(state, move->gravity);
		break;
	case move_type_drop:
		state->bags[state->turn] -= 1;
		cell = state->board->drop_cells[state->gravity][move->drop_index];
		cell = state_get_empty(state, cell);
		state->tokens[cell - state->board->cells] = move->token;
		state_update_at(state, cell);
		break;
	}
	state->turn = !state->turn;
}
