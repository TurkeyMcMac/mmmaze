#include "player.h"
#include "grow.h"
#include "maze.h"
#include "tile.h"
#include <stdlib.h>

struct heads {
	struct head {
		short x, y;
	} *arr;
	size_t len, cap;
};

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

int player_mark_seen(struct maze *maze, int x, int y, int dist)
{
	int ret = -1;
	struct heads heads = { NULL, 0, 0 };
	if (add_head(maze, x, y, &heads)) goto error;

	while (dist-- > 0) {
		size_t i = heads.len;
		while (i-- > 0) {
			int hx = heads.arr[i].x, hy = heads.arr[i].y;
			TILE_TYPE t = MAZE_GET(maze, hx, hy);
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
			heads.arr[i] = heads.arr[--heads.len];
		}
	}

	ret = 0;
error:
	free(heads.arr);
	return ret;
}

int player_unmark_seen(struct maze *maze, int x, int y, int dist)
{
	int x_, y_;
	(void)x;
	(void)y;
	(void)dist;
	for (y_ = 0; y_ < maze->height; ++y_) {
		for (x_ = 0; x_ < maze->width; ++x_) {
			MAZE_GET(maze, x_, y_) &= ~BIT_PLAYER_SEEN;
		}
	}
	return 0;
}
