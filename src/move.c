#include "move.h"

TILE_TYPE move_get_available(const struct maze *maze, int x, int y)
{
	TILE_TYPE avail = MAZE_GET(maze, x, y)
		& (BIT_RIGHT | BIT_UP | BIT_LEFT | BIT_DOWN);
	if ((avail & BIT_RIGHT) && (MAZE_GET(maze, x + 1, y) & BIT_OBSTRUCTED))
		avail &= ~BIT_RIGHT;
	if ((avail & BIT_UP) && (MAZE_GET(maze, x, y - 1) & BIT_OBSTRUCTED))
		avail &= ~BIT_UP;
	if ((avail & BIT_LEFT) && (MAZE_GET(maze, x - 1, y) & BIT_OBSTRUCTED))
		avail &= ~BIT_LEFT;
	if ((avail & BIT_DOWN) && (MAZE_GET(maze, x, y + 1) & BIT_OBSTRUCTED))
		avail &= ~BIT_DOWN;
	return avail;
}
