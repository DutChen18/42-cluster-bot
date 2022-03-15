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
			winning = &moves[i];
			return true;
		}
		if (not_losing == NULL)
		{
			size_t size;
			move_t *new_moves = move_gen(&size, &one_turn, -1, -1);
			for(size_t i = 0; i < move_count; i++)
			{
				state_copy(&two_turn, &one_turn);
				state_move(&one_turn, &moves[i]);
				if (state_winner(&one_turn) == state->turn)
				{
					not_losing = &moves[i];
					break ;
				}
			}
		}
	}
	winning = not_losing;
	return false;
}

void search(state_t *state, move_t *move, int8_t token_a, int8_t token_b)
{
	size_t size;
	move_t *moves = move_gen(&size, state, token_a, token_b);
	random_t rng;
	random_new(&rng);
	is_winning_move(state, moves, size, move);
	alloc_free(&alloc, moves); //volgensmij moet ik dingen freeen
}
