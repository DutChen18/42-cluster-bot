#include "bot.h"
#include <stdlib.h>
#include <assert.h>

static void board_init_neighbors(board_t *board)
{
	for (size_t i = 0; i < board->cell_count; i += 1) {
		cell_t *cell = &board->cells[i];
		cell->neighbors[0] = board_get(board, cell->coord.q, cell->coord.r - 1, cell->coord.s + 1);
		cell->neighbors[1] = board_get(board, cell->coord.q + 1, cell->coord.r - 1, cell->coord.s);
		cell->neighbors[2] = board_get(board, cell->coord.q + 1, cell->coord.r, cell->coord.s - 1);
		cell->neighbors[3] = board_get(board, cell->coord.q, cell->coord.r + 1, cell->coord.s - 1);
		cell->neighbors[4] = board_get(board, cell->coord.q - 1, cell->coord.r + 1, cell->coord.s);
		cell->neighbors[5] = board_get(board, cell->coord.q - 1, cell->coord.r, cell->coord.s + 1);
	}
}

static void get_drop_cells(board_t *board, config_t *config, int gravity)
{
	board->drop_cell_count[gravity] = 0;
	for (int pos = -config->grid_size + 1; pos < config->grid_size; pos += 1) {
		coord_t cur_coords = coord_from_pos(pos, gravity, config->grid_size);
		cell_t *cell = board_get(board, cur_coords.q, cur_coords.r, cur_coords.s);
		if (cell != NULL) {
			board->drop_cells[gravity] = realloc(board->drop_cells[gravity], (board->drop_cell_count[gravity] + 1) * sizeof(cell_t*));
			board->drop_cells[gravity][board->drop_cell_count[gravity]] = cell;
			board->drop_cell_count[gravity] += 1;
		}
	}
}

static void get_gravity_cells(board_t *board, config_t *config, int gravity)
{
	(void) config;
	board->gravity_cell_count[gravity] = 0;
	for (size_t i = 0; i < board->cell_count; i += 1) {
		if (board->cells[i].neighbors[gravity] == NULL) {
			board->gravity_cells[gravity] = realloc(board->gravity_cells[gravity], (board->gravity_cell_count[gravity] + 1) * sizeof(cell_t));
			board->gravity_cells[gravity][board->gravity_cell_count[gravity]] = &board->cells[i];
			board->gravity_cell_count[gravity] += 1;
		}
	}
}

cell_t *board_get(board_t *board, int q, int r, int s)
{
	for (size_t i = 0; i < board->cell_count; i += 1) {
		cell_t *cell = &board->cells[i];
		if (cell->coord.q == q && cell->coord.r == r && cell->coord.s == s)
			return cell;
	}
	return NULL;
}

void board_new(board_t *board, config_t *config)
{
	size_t i = 0;

	board->cell_count = (config->grid_size * config->grid_size - config->grid_size) * 3 + 1 - config->wall_count;
	board->cells = malloc(sizeof(*board->cells) * board->cell_count);
	board->config = config;
	for (int q = -config->grid_size + 1; q < config->grid_size; q += 1) {
		for (int r = -config->grid_size + 1; r < config->grid_size; r += 1) {
			for (int s = -config->grid_size + 1; s < config->grid_size; s += 1) {
				if (q + r + s == 0) {
					for (size_t j = 0; j < config->wall_count; j += 1) {
						if (q == config->walls[j].q && r == config->walls[j].r && s == config->walls[j].s)
							goto skip_adding ;
					}
					cell_t *cell = &board->cells[i];
					cell->coord.q = q;
					cell->coord.r = r;
					cell->coord.s = s;
					i += 1;
				skip_adding:
					(void) 0;
				}
			}
		}
	}
	assert(board->cell_count == i);
	board_init_neighbors(board);
	for (int gravity = 0; gravity < 6; gravity += 1) {
		board->drop_cells[gravity] = NULL;
		board->gravity_cells[gravity] = NULL;
		get_drop_cells(board, config, gravity);
		get_gravity_cells(board, config, gravity);
	}
}
