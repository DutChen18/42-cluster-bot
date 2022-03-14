#include "bot.h"

int coord_to_pos(coord_t coord, gravity_t gravity)
{
	switch (gravity) {
	case north:
		return -coord.q;
	case north_east:
		return coord.s;
	case south_east:
		return -coord.r;
	case south:
		return coord.q;
	case south_west:
		return -coord.s;
	case north_west:
		return coord.r;
	}
}

coord_t coord_from_pos(int pos, gravity_t gravity, int size)
{
	int q = -pos;
	int r = size < size + pos ? size : size + pos;
	int s = -(q + r);
	switch (gravity) {
	case north:
		return (coord_t) { .q = q, .r = r, .s = s };
	case north_east:
		return (coord_t) { .q = -r, .r = -s, .s = -q };
	case south_east:
		return (coord_t) { .q = s, .r = q, .s = r };
	case south:
		return (coord_t) { .q = -q, .r = -r, .s = -s };
	case south_west:
		return (coord_t) { .q = r, .r = s, .s = q };
	case north_west:
		return (coord_t) { .q = -s, .r = -q, .s = -r };
	}
}
