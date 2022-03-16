#include "bot.h"
#include <stdlib.h>

static const minimax_t params[] = {
	{ .mm_depth = 2, .ev_samples = 0 },
	{ .mm_depth = 2, .ev_samples = 50 },
	{ .mm_depth = 2, .ev_samples = 200 },
};

static void search_random(state_t *state, int *winner, int *depth, int max_depth)
{
	*depth += 1;
	*winner = state_winner(state);
	if (*winner != -1)
		return;
	if (max_depth == 0) {
		*winner = -1;
		return;
	}
	size_t move_count;
	move_t *moves = move_gen(&move_count, state, (tokens_t) { .a = -1, .b = -1 });
	state_move(state, &moves[random_next(&rng) % move_count]);
	alloc_free(&alloc, moves);
	search_random(state, winner, depth, max_depth - 1);
}

static float search_eval(state_t *state, int samples, int turn, int max_depth)
{
	float score = 0;
	int winner = state_winner(state);
	if (winner == turn)
		score += 1.0;
	else if (winner == !turn)
		score -= 1.0;
	if (samples == 0 || winner != -1)
		return score;
	for (int i = 0; i < samples; i++) {
		state_t one_turn;
		int winner, depth = 0;
		state_copy(&one_turn, state);
		search_random(&one_turn, &winner, &depth, max_depth);
		state_delete(&one_turn);
		if (winner == turn)
			score += 1.0 / depth;
		else if (winner == !turn)
			score -= 1.0 / depth;
	}
	return score / samples;
}

static float heuristics_cluster(const minimax_t *mm, state_t *state)
{
	return search_eval(state, mm->ev_samples, state->turn, 10);
}

static float minimax_cluster(const minimax_t *mm, search_t *s, state_t *state, int depth, float alpha, float beta)
{
	size_t size;
	state_t child;

	if (get_time() >= state->board->config->timeout * 0.8) {
		s->timeout = true;
		return 0;
	}
	if (depth == 0 || state_winner(state) >= 0)
		return (heuristics_cluster(mm, state));
	
	move_t *moves = move_gen(&size, state, (tokens_t) { .a = -1, .b = -1 });
	float score = -1000000.0f;
	for (size_t i = 0; i < size; i++) {
		state_copy(&child, state);
		state_move(&child, &moves[i]);
		float eval = -minimax_cluster(mm, s, &child, depth - 1, -beta, -alpha);
		state_delete(&child);
		if (s->timeout)
			break;
		score = eval > score ? eval : score;
		alpha = alpha > score ? alpha : score;
		if (alpha >= beta)
			break;
	}
	alloc_free(&alloc, moves);
	return (score);
}

static void outer_move_minimax(const minimax_t *mm, search_t *s, state_t *state, tokens_t token, int depth)
{
	size_t	size;
	state_t child;
	move_t	*moves = move_gen(&size, state, token);

	float score = -1000000.0f;
	s->move = moves[0];
	for (size_t i = 0; i < size; i++) {
		state_copy(&child, state);
		state_move(&child, &moves[i]);
		float eval = -minimax_cluster(mm, s, &child, depth - 1, -1000000.0f, -score);
		state_delete(&child);
		if (s->timeout)
			break;
		if (eval > score) {
			score = eval;
			s->move = moves[i];
		}
	}
	alloc_free(&alloc, moves);
}

void search(state_t *state, move_t *move, tokens_t token)
{
	search_t s;

	for (size_t i = 0; i < sizeof(params) / sizeof(*params); i++) {
		s.timeout = false;
		outer_move_minimax(&params[i], &s, state, token, params[i].mm_depth);
		if (!s.timeout)
			*move = s.move;
		else
			break;
	}
}
