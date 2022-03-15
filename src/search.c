#include "bot.h"
#include <stdlib.h>
#include <stdio.h>

static void search_random(state_t *state, int *winner, int *depth)
{
	*depth += 1;
	*winner = state_winner(state);
	if (*winner != -1)
		return;
	size_t move_count;
	move_t *moves = move_gen(&move_count, state, (tokens_t) { .a = -1, .b = -1 });
	state_move(state, &moves[random_next(&rng) % move_count]);
	alloc_free(&alloc, moves);
	search_random(state, winner, depth);
}

static float search_eval(state_t *state, int samples, int turn)
{
	float score = 0;
	for (int i = 0; i < samples; i++) {
		state_t one_turn;
		state_copy(&one_turn, state);
		int winner, depth = 0;
		search_random(&one_turn, &winner, &depth);
		state_delete(&one_turn);
		if (winner == turn)
			score += 100.0 / depth / depth;
		else if (winner == !turn)
			score -= 100.0 / depth / depth;
	}
	return score / samples;
}

int	heuristics_cluster(state_t *state)
{
	return search_eval(state, 20, state->turn) * 10000;
}

int	minmax_cluster(state_t *state, int depth, int alpha, int beta)
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
	
	score = INT32_MIN;
	for (size_t i = 0; i < size; i++)
	{
		state_copy(&child, state);
		state_move(&child, &moves[i]);
		eval = -minmax_cluster(&child, depth - 1, -beta, -alpha);
		score = eval > score ? eval : score;
		alpha = alpha > score ? alpha : score;
		if (alpha >= beta)
			break;
		state_delete(&child);
	}
	alloc_free(&alloc, moves);
	return (score);
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
		eval = -minmax_cluster(&child, depth - 1, score, INT32_MAX);
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
	outer_move_minmax(state, token, 2, move);
}
