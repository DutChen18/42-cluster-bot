#include "bot.h"

bool	is_winning_move(state_t *state, move_t *moves, size_t move_count, move_t *winning)
{
	state_t	one_turn;
	state_t	two_turn;
	move_t	*not_losing;

	not_losing = NULL;
	for (size_t i = 0; i < move_count; i++)
	{
		state_copy(&one_turn, state);
		state_move(&one_turn, &moves[i]);
		if (state_winner(&one_turn) == state->turn)
		{
			*winning = moves[i];
			state_delete(&one_turn);
			return true;
		}
		if (not_losing == NULL)
		{
			size_t size;
			move_t *new_moves = move_gen(&size, &one_turn, -1, -1);
			for (size_t j = 0; j < size; j++)
			{
				state_copy(&two_turn, &one_turn);
				state_move(&two_turn, &new_moves[j]);
				if (state_winner(&two_turn) == one_turn.turn)
				{
					state_delete(&two_turn);
					goto skip_not_losing;
				}
				state_delete(&two_turn);
			}
			not_losing = &moves[i];
		skip_not_losing:
			alloc_free(&alloc, new_moves);
		}
		state_delete(&one_turn);
	}
	if (not_losing != NULL)
		*winning = *not_losing;
	else
		*winning = moves[0];
	return false;
}

void search(state_t *state, move_t *move, int8_t token_a, int8_t token_b)
{
	size_t size;
	move_t *moves = move_gen(&size, state, token_a, token_b);
	random_t rng;
	random_new(&rng);
	is_winning_move(state, moves, size, move);
	alloc_free(&alloc, moves);
}
