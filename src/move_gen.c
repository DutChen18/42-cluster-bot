#include "bot.h"

move_t *move_gen(size_t *size, state_t *state, tokens_t tokens)
{
	move_t *result = alloc_alloc(&alloc, sizeof(*result) * (6 + state->board->drop_cell_count[state->gravity] * state->board->config->color_count));
	size_t index = 0;
	for (size_t i = 0; i < state->board->drop_cell_count[state->gravity]; i++) {
		for (int j = 0; j < state->board->config->color_count; j++) {
			int k = j + state->turn * state->board->config->color_count;
			if ((tokens.a == -1 && tokens.b == -1) || k == tokens.a || k == tokens.b) {
				if (state_token(state, state->board->drop_cells[state->gravity][i]) == -1) {
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
