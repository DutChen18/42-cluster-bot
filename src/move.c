#include "bot.h"

void state_set_gravity(state_t *state, gravity_t gravity)
{
	state->gravity = gravity;
	for (size_t i = 0; i < state->board->gravity_cell_count[gravity]; i++) {
		cell_t *top = state->board->gravity_cells[gravity][i], *bot = top;
		while (top != NULL) {
			if (state->tokens[top - state->board->cells] != -1) {
				int8_t tmp = state->tokens[bot - state->board->cells];
				state->tokens[bot - state->board->cells] = state->tokens[top - state->board->cells];
				state->tokens[top - state->board->cells] = tmp;
				bot = bot->neighbors[!gravity];
			}
			top = top->neighbors[!gravity];
		}
	}
}

cell_t *state_get_empty(state_t *state, cell_t *cell)
{
	while (cell->neighbors[state->gravity] != NULL && state->tokens[cell - state->board->cells] == -1)
		cell = cell->neighbors[state->gravity];
	return cell;
}

int state_winner(state_t *state)
{
	int best_player = -1;
	int best_length = 0;
	int best_count = 0;
	for (int i = 0; i < 3; i++) {
		for (size_t j = 0; j < state->board->cell_count; j++) {
			if (state->tokens[j] != -1) {
				int length = 1;
				cell_t *cell = &state->board->cells[j];
				while (cell->neighbors[i] != NULL && state->tokens[cell->neighbors[i] - state->board->cells] == state->tokens[j]) {
					cell = cell->neighbors[i];
					length += 1;
				}
				if (length > best_length) {
					best_player = state->tokens[j] / state->board->config->color_count;
					best_length = length;
					best_count = 1;
				} else if (length == best_length && state->tokens[j] / state->board->config->color_count != best_player) {
					best_count += 1;
				}
			}
		}
	}
	if (best_length >= state->board->config->win_length && best_count == 1)
		return best_player;
	return -1;
}

void state_make_move(moved_state_t *moved_state, state_t *state, move_t *move)
{
	switch (move->type) {
	case move_type_rotate:
		state_copy(&moved_state->tmp, state);
		moved_state->state = &moved_state->tmp;
		moved_state->state->bags[moved_state->state->turn] -= 2;
		state_set_gravity(moved_state->state, move->gravity);
		break;
	case move_type_drop:
		moved_state->state = state;
		moved_state->state->bags[moved_state->state->turn] -= 1;
		moved_state->cell = state_get_empty(state, state->board->drop_cells[state->gravity][move->drop_index]);
		break;
	}
	moved_state->state->turn = !moved_state->state->turn;
}

void state_unmake_move(moved_state_t *moved_state)
{
	switch (moved_state->type) {
	case move_type_rotate:
		state_delete(&moved_state->tmp);
		break;
	case move_type_drop:
		moved_state->state->tokens[moved_state->cell - moved_state->state->board->cells] = -1;
		moved_state->state->turn = !moved_state->state->turn;
		moved_state->state->bags[moved_state->state->turn] += 1;
		break;
	}
}
