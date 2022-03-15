#include "bot.h"

move_t *move_gen(size_t *size, state_t *state, int8_t token_a, int8_t token_b)
{
	move_t *result = alloc_alloc(&alloc, sizeof(*result) * (6 + state->board->drop_cell_count[state->gravity] * state->board->config->color_count));
	size_t index = 0;
	for (size_t i = 0; i < state->board->drop_cell_count[state->gravity]; i++) {
		for (int j = 0; j < state->board->config->color_count; j++) {
			int k = j + state->turn * state->board->config->color_count;
			if ((token_a == -1 && token_b == -1) || k == token_a || k == token_b) {
				if (state->tokens[state->board->drop_cells[state->gravity][i] - state->board->cells] == -1) {
					result[index].type = move_type_drop;
					result[index].drop_index = i;
					result[index].token = k;
					index += 1;
				}
			}
		}
	}
	if (state->bags[state->turn] >= 2) {
		for (int i = 0; i < 6; i++) {
			result[index].type = move_type_rotate;
			result[index].gravity = i;
			index += 1;
		}
	}
	*size = index;
	return result;
}
