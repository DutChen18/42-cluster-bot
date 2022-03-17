#include "bot.h"
#include <stdlib.h>

static const minimax_t params[] = {
	// { .mcts_eval = true, .mm_depth = 2, .ev_samples = 0 },
	// { .mcts_eval = true, .mm_depth = 2, .ev_samples = 50 },
	// { .mcts_eval = true, .mm_depth = 2, .ev_samples = 200 },
	// { .mcts_eval = true, .mm_depth = 4, .ev_samples = 20 },
	{ .mcts_eval = false, .mm_depth = 2 },
	{ .mcts_eval = false, .mm_depth = 3 },
	{ .mcts_eval = false, .mm_depth = 4 },
	{ .mcts_eval = false, .mm_depth = 5 },
	{ .mcts_eval = false, .mm_depth = 6 },
	{ .mcts_eval = false, .mm_depth = 7 },
	{ .mcts_eval = false, .mm_depth = 8 },
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

static float mcts_search_eval(state_t *state, int samples, int turn, int max_depth)
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

static float state_length_score(state_t *state, cell_t *cell, gravity_t gravity)
{
	float length = 0;
	cell_t *next = cell;
	while (next->neighbors[gravity] != NULL && state_token(state, next->neighbors[gravity]) == state_token(state, cell)) {
		next = next->neighbors[gravity];
		length += 5.0;
	}
	if (next->neighbors[gravity] != NULL && state_token(state, next->neighbors[gravity]) == -1) {
		length += 2.0;
	}
	return length;
}

static float deterministic_search_eval(state_t *state, int depth)
{
	int winner = state_winner(state);
	if (winner == state->turn)
		return 1000000.0f + depth;
	else if (winner == !state->turn)
		return -1000000.0f - depth;
	float score = 0;
	for (size_t i = 0; i < state->board->cell_count; i++) {
		if (state->tokens[i] == -1)
			continue;
		float tmp = 0;
		for (int j = 0; j < 3; j++) {
			tmp += state_length_score(state, &state->board->cells[i], j);
			tmp += state_length_score(state, &state->board->cells[i], j + 3);
		}
		if (state->tokens[i] / state->board->config->color_count == state->turn)
			score += tmp;
		else
			score -= tmp;
	}
	score += state->bags[state->turn] * 10;
	score -= state->bags[!state->turn] * 10;
	return score + (random_next(&rng) % 1000) / 1000.0f;
}

static float heuristics_cluster(const minimax_t *mm, state_t *state, int depth)
{
	if (mm->mcts_eval)
		return mcts_search_eval(state, mm->ev_samples, state->turn, 10);
	else
		return deterministic_search_eval(state, depth);
}

static float minimax_cluster(const minimax_t *mm, search_t *s, state_t *state, int depth, float alpha, float beta)
{
	size_t size;
	state_t child;

	if (get_time() >= state->board->config->timeout * 0.8 - 0.01) {
		s->timeout = true;
		return 0;
	}
	if (depth == 0 || state_winner(state) >= 0)
		return (heuristics_cluster(mm, state, depth));
	
	move_t *moves = move_gen(&size, state, (tokens_t) { .a = -1, .b = -1 });
	float score = -1000000000.0f;
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

	float score = -1000000000.0f;
	s->move = moves[0];
	for (size_t i = 0; i < size; i++) {
		state_copy(&child, state);
		state_move(&child, &moves[i]);
		float eval = -minimax_cluster(mm, s, &child, depth - 1, -1000000000.0f, -score);
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
