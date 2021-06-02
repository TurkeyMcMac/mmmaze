#include "player.h"
#include "grow.h"
#include "maze.h"
#include "tile.h"
#include <stdlib.h>

/* The list of player sight reveal heads. */
struct heads {
	struct head {
		unsigned char x, y;
	} *arr;
	size_t len, cap;
};

/* Adds a head to the maze if it has not yet been seen. */
static int add_head(struct maze *maze, int x, int y, struct heads *heads)
{
	TILE_TYPE *t = &MAZE_GET(maze, x, y);
	if (!(*t & BIT_PLAYER_SEEN)) {
		struct head *h =
			GROW(heads->arr, heads->len, heads->cap);
		if (!h) return -1;
		*t |= BIT_PLAYER_SEEN;
		h->x = x;
		h->y = y;
	}
	return 0;
}

int plyr_mark_seen(struct maze *maze, int x, int y, int dist)
{
	int ret = -1;
	struct heads heads = { NULL, 0, 0 };
	if (add_head(maze, x, y, &heads)) goto error;

	/* Keep revealing for the given distance. */
	while (dist-- > 0) {
		/* Extend each of the heads. */
		size_t i = heads.len;
		while (i-- > 0) {
			int hx = heads.arr[i].x, hy = heads.arr[i].y;
			TILE_TYPE t = MAZE_GET(maze, hx, hy);
			/* Add all possible heads around this one. */
			if ((t & BIT_RIGHT)
			 && add_head(maze, hx + 1, hy, &heads))
				goto error;
			if ((t & BIT_UP)
			 && add_head(maze, hx, hy - 1, &heads))
				goto error;
			if ((t & BIT_LEFT)
			 && add_head(maze, hx - 1, hy, &heads))
				goto error;
			if ((t & BIT_DOWN)
			 && add_head(maze, hx, hy + 1, &heads))
				goto error;
			/* Remove the head. */
			heads.arr[i] = heads.arr[--heads.len];
		}
	}

	ret = 0;
error:
	free(heads.arr);
	return ret;
}

int plyr_unmark_seen(struct maze *maze, int x, int y, int dist)
{
	/* A few extra tiles are cleared. */
	int start_x = x - dist, start_y = y - dist;
	int end_x = x + dist, end_y = y + dist;
	if (start_x < 0) start_x = 0;
	if (start_y < 0) start_y = 0;
	if (end_x >= maze->width) end_x = maze->width - 1;
	if (end_y >= maze->height) end_y = maze->height - 1;
	for (y = start_y; y <= end_y; ++y) {
		for (x = start_x; x <= end_x; ++x) {
			MAZE_GET(maze, x, y) &= ~BIT_PLAYER_SEEN;
		}
	}
	return 0;
}
