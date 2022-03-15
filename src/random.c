#include "bot.h"
#include <time.h>
#include <unistd.h>

random_t rng;

static uint32_t mix(uint32_t a, uint32_t b, uint32_t c)
{
	a = (a - b - c) ^ (c >> 13);
	b = (b - c - a) ^ (a << 8);
	c = (c - a - b) ^ (b >> 13);
	a = (a - b - c) ^ (c >> 12);
	b = (b - c - a) ^ (a << 16);
	c = (c - a - b) ^ (b >> 5);
	a = (a - b - c) ^ (c >> 3);
	b = (b - c - a) ^ (a << 10);
	c = (c - a - b) ^ (b >> 15);
	return c;
}

void random_new(random_t *rng)
{
	rng->state = mix(time(NULL), clock(), getpid());
}

void random_with_seed(random_t *rng, uint32_t seed)
{
	rng->state = seed;
}

int32_t random_next(random_t *rng)
{
	int32_t result = (rng->state >> 16) & 0x7FFF;
	rng->state = rng->state * 1103515245 + 12345;
	return result;
}
