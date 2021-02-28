#include "monster.h"
#include "move.h"
#include <stddef.h>

void monster_init(struct monster *monster, int x, int y, struct maze *maze)
{
	monster->x = x;
	monster->y = y;
	monster->back_dir = 0;
	monster->next_move = 0;
	MAZE_GET(maze, x, y) |= BIT_MONSTER;
}

void monster_start_move_random(struct monster *monster, const struct maze *maze,
	RAND_TYPE *rand)
{
	size_t n_avail = 0;
	TILE_TYPE avail[4];
	TILE_TYPE avail_bits = move_get_available(maze, monster->x, monster->y)
		& ~monster->back_dir;
	if (avail_bits & BIT_RIGHT) avail[n_avail++] = BIT_RIGHT;
	if (avail_bits & BIT_UP) avail[n_avail++] = BIT_UP;
	if (avail_bits & BIT_LEFT) avail[n_avail++] = BIT_LEFT;
	if (avail_bits & BIT_DOWN) avail[n_avail++] = BIT_DOWN;

	if (n_avail > 0) {
		monster->next_move = avail[rand_gen(rand) % n_avail];
	} else {
		monster->next_move = monster->back_dir;
		monster->back_dir = 0;
	}
}

void monster_make_move(struct monster *monster, struct maze *maze)
{
	MAZE_GET(maze, monster->x, monster->y) &= ~BIT_MONSTER;

	switch (monster->next_move) {
	case BIT_RIGHT:
		if (!(MAZE_GET(maze, monster->x + 1, monster->y) & BIT_MONSTER))
		{
			monster->back_dir = BIT_LEFT;
			++monster->x;
		}
		break;
	case BIT_UP:
		if (!(MAZE_GET(maze, monster->x, monster->y - 1) & BIT_MONSTER))
		{
			monster->back_dir = BIT_DOWN;
			--monster->y;
		}
		break;
	case BIT_LEFT:
		if (!(MAZE_GET(maze, monster->x - 1, monster->y) & BIT_MONSTER))
		{
			monster->back_dir = BIT_RIGHT;
			--monster->x;
		}
		break;
	case BIT_DOWN:
		if (!(MAZE_GET(maze, monster->x, monster->y + 1) & BIT_MONSTER))
		{
			monster->back_dir = BIT_UP;
			++monster->y;
		}
		break;
	}

	monster->next_move = 0;

	MAZE_GET(maze, monster->x, monster->y) |= BIT_MONSTER;
}
