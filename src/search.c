#include "bot.h"

void search(state_t *state, move_t *move, int8_t token_a, int8_t token_b)
{
	size_t size;
	move_t *moves = move_gen(&size, state, token_a, token_b);
	random_t rng;
	random_new(&rng);
	*move = moves[random_next(&rng) % size];
	alloc_free(&alloc, moves);
}
