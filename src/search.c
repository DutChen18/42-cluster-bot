#include "bot.h"
#include <stdlib.h>

int	heuristics_cluster(state_t *state)
{
	int	score;

	score = state_winner(state);
	if (score != -1)
	{
		if (score == state->board->config->bot_id)
			return (INT32_MAX);
		else
			return (INT32_MIN);
	}
	//toevoegingen voor heuristics
	return (score);
}

int	minmax_cluster(state_t *state, int depth, bool maximizing)
{
	size_t	size;
	int		eval;
	state_t child;
	int		score;
	move_t	*moves = move_gen(&size, state, (tokens_t) { .a = -1, .b = -1 });

	if (depth == 0 || state_winner(state) >= 0)
	{
		return (heuristics_cluster(state));
	}
	
	if (maximizing)
	{
		score = INT32_MIN;
		for (size_t i = 0; i < size; i++)
		{
			state_copy(&child, state);
			state_move(&child, &moves[i]);
			eval = minmax_cluster(&child, depth - 1, false);
			score = eval > score ? eval : score;
			state_delete(&child);
		}
		alloc_free(&alloc, moves);
		return (score);
	}
	else
	{
		score = INT32_MAX;
		for (size_t i = 0; i < size; i++)
		{
			state_copy(&child, state);
			state_move(&child, &moves[i]);
			eval = minmax_cluster(&child, depth - 1, true);
			score = eval < score ? eval : score;
			state_delete(&child);
		}
		alloc_free(&alloc, moves);
		return (score);
	}
}

void	outer_move_minmax(state_t *state, tokens_t token, int depth, move_t *best_move)
{
	size_t	size;
	int		eval;
	state_t child;
	int		score;
	move_t	*moves = move_gen(&size, state, token);

	score = INT32_MIN;
	*best_move = moves[0];
	for (size_t i = 0; i < size; i++)
	{
		state_copy(&child, state);
		state_move(&child, &moves[i]);
		eval = minmax_cluster(&child, depth - 1, false);
		if (eval > score)
		{
			score = eval;
			*best_move = moves[i];
		}
		state_delete(&child);
	}
	alloc_free(&alloc, moves);
}

void search(state_t *state, move_t *move, tokens_t token)
{
	// size_t size;
	// move_t *moves = move_gen(&size, state, token);
	// random_t rng;
	// random_new(&rng);
	outer_move_minmax(state, token, 4, move);
}
