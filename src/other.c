#include "other.h"
#include "bits.h"
#include <stddef.h>

void other_move_random(struct other *other, const struct maze *maze,
	RAND_TYPE *rand)
{
	TILE_TYPE move;
	TILE_TYPE avail = MAZE_GET(maze, other->x, other->y)
		& (BIT_RIGHT | BIT_UP | BIT_LEFT | BIT_DOWN)
		& ~other->back_dir;
	if (avail == 0) {
		move = other->back_dir;
	} else if (avail == BIT_RIGHT || avail == BIT_UP
	 || avail == BIT_LEFT || avail == BIT_DOWN) {
		move = avail;
	} else {
		size_t n_avail = 0;
		TILE_TYPE avail_arr[4];
		if (avail & BIT_RIGHT) avail_arr[n_avail++] = BIT_RIGHT;
		if (avail & BIT_UP) avail_arr[n_avail++] = BIT_UP;
		if (avail & BIT_LEFT) avail_arr[n_avail++] = BIT_LEFT;
		if (avail & BIT_DOWN) avail_arr[n_avail++] = BIT_DOWN;
		move = avail_arr[rand_gen(rand) % n_avail];
	}
	switch (move) {
	case BIT_RIGHT:
		other->back_dir = BIT_LEFT;
		++other->x;
		break;
	case BIT_UP:
		other->back_dir = BIT_DOWN;
		--other->y;
		break;
	case BIT_LEFT:
		other->back_dir = BIT_RIGHT;
		--other->x;
		break;
	case BIT_DOWN:
		other->back_dir = BIT_UP;
		++other->y;
		break;
	}
}
