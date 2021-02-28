#include "other.h"
#include "move.h"
#include <stddef.h>

void other_init(struct other *other, int x, int y, struct maze *maze)
{
	other->x = x;
	other->y = y;
	other->back_dir = 0;
	other->next_move = 0;
	MAZE_GET(maze, x, y) |= BIT_OBSTRUCTED;
}

void other_start_move_random(struct other *other, const struct maze *maze,
	RAND_TYPE *rand)
{
	size_t n_avail = 0;
	TILE_TYPE avail[4];
	TILE_TYPE avail_bits = move_get_available(maze, other->x, other->y)
		& ~other->back_dir;
	if (avail_bits & BIT_RIGHT) avail[n_avail++] = BIT_RIGHT;
	if (avail_bits & BIT_UP) avail[n_avail++] = BIT_UP;
	if (avail_bits & BIT_LEFT) avail[n_avail++] = BIT_LEFT;
	if (avail_bits & BIT_DOWN) avail[n_avail++] = BIT_DOWN;

	if (n_avail > 0) {
		other->next_move = avail[rand_gen(rand) % n_avail];
	} else {
		other->next_move = other->back_dir;
		other->back_dir = 0;
	}
}

void other_make_move(struct other *other, struct maze *maze)
{
	MAZE_GET(maze, other->x, other->y) &= ~BIT_OBSTRUCTED;

	switch (other->next_move) {
	case BIT_RIGHT:
		if (!(MAZE_GET(maze, other->x + 1, other->y) & BIT_OBSTRUCTED))
		{
			other->back_dir = BIT_LEFT;
			++other->x;
		}
		break;
	case BIT_UP:
		if (!(MAZE_GET(maze, other->x, other->y - 1) & BIT_OBSTRUCTED))
		{
			other->back_dir = BIT_DOWN;
			--other->y;
		}
		break;
	case BIT_LEFT:
		if (!(MAZE_GET(maze, other->x - 1, other->y) & BIT_OBSTRUCTED))
		{
			other->back_dir = BIT_RIGHT;
			--other->x;
		}
		break;
	case BIT_DOWN:
		if (!(MAZE_GET(maze, other->x, other->y + 1) & BIT_OBSTRUCTED))
		{
			other->back_dir = BIT_UP;
			++other->y;
		}
		break;
	}

	other->next_move = 0;

	MAZE_GET(maze, other->x, other->y) |= BIT_OBSTRUCTED;
}
