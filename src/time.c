#include "bot.h"

game_time_t g_time;

void	time_init(void)
{
	struct	timeval	current_time;

	gettimeofday(&current_time, NULL);
	g_time.start.tv_sec = current_time.tv_sec;
	g_time.start.tv_usec = current_time.tv_usec;
}

float	get_time(void)
{
	struct timeval	current_time;
	float			return_time;

	gettimeofday(&current_time, NULL);
	current_time.tv_sec -= g_time.start.tv_sec;
	current_time.tv_usec -= g_time.start.tv_usec;
	return_time = (float) current_time.tv_sec;
	return_time += (float) current_time.tv_usec / (float) 1000000;
	return (return_time);
}

