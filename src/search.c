#include "bot.h"

int	heuristics_cluster(state_t *state)
{
	int	score;

	score = state_winner(state);
	if (score != -1)
	{
		if (score == 0)
			return (INT32_MIN);
		else
			return (INT32_MAX);
	}
	//toevoegingen voor heuristics
	return (score);
}

int	minmax_cluster(state_t *state, tokens_t token, int depth, bool maximizing)
{
	size_t			size;
	int				eval;
	state_t 		child;
	int				score;
	move_t			*moves = move_gen(&size, state, (tokens_t) { .a = -1, .b = -1});

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
			eval = min_max_cluster(&child, (tokens_t) { .a = -1, .b = -1}, depth - 1, false);
			score = max(eval, score);
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
			eval = min_max_cluster(&child, (tokens_t) { .a = -1, .b = -1}, depth - 1, false);
			score = min(eval, score);
			state_delete(&child);
		}
		alloc_free(&alloc, moves);
		return (score);
	}
}

void	outer_move_minmax(state_t *state, tokens_t token, int depth, move_t *best_move)
{
		size_t		size;
	int				eval;
	state_t 		child;
	int				score;
	move_t			best_move;
	bool			best_move_set;
	move_t			*moves = move_gen(&size, state, (tokens_t) { .a = -1, .b = -1});

	score = INT32_MIN;
	best_move_set = false;
	for (size_t i = 0; i < size; i++)
	{
		state_copy(&child, state);
		state_move(&child, &moves[i]);
		eval = min_max_cluster(&child, (tokens_t) { .a = -1, .b = -1}, depth - 1, false);
		if (score > eval)
		{
			eval = score;
			*best_move = moves[i];
			best_move_set = true;
		}
		state_delete(&child);
	}
	if (best_move_set == false)
		*best_move = moves[0];
	alloc_free(&alloc, moves);
	return (best_move);
}

void search(state_t *state, move_t *move, tokens_t token)
{
	// size_t size;
	// move_t *moves = move_gen(&size, state, token);
	// random_t rng;
	// random_new(&rng);
	outer_move_minmax(state, token, 4, &move);
}
