#include "maze.h"
#include "bits.h"
#include "grow.h"
#include <stdlib.h>

#define LOOPINESS (MAX_RAND / 10)

struct heads {
	struct head {
		short x, y;
	} *arr;
	size_t len, cap;
};

static int coords_are_ok(const struct maze *maze, int x, int y)
{
	return x >= 0 && y >= 0 && x < maze->width && y < maze->height;
}


static int add_head(const struct maze *maze, int x, int y, struct heads *heads)
{
	if (coords_are_ok(maze, x, y)) {
		TILE_TYPE *t = &MAZE_GET(maze, x, y);
		if ((*t & BIT_CHECKED) == 0) {
			struct head *h =
				GROW(heads->arr, heads->len, heads->cap);
			if (!h) return -1;
			h->x = x;
			h->y = y;
			*t |= BIT_CHECKED;
		}
	}
	return 0;
}

static int add_heads(const struct maze *maze, int x, int y, struct heads *heads)
{
	if (add_head(maze, x + 1, y, heads)
	 || add_head(maze, x, y - 1, heads)
	 || add_head(maze, x - 1, y, heads)
	 || add_head(maze, x, y + 1, heads))
		return -1;
	return 0;
}

static void rand_point(const struct maze *maze, RAND_TYPE *rand, int *x, int *y)
{
	*x = (unsigned)rand_gen(rand) % maze->width;
	*y = (unsigned)rand_gen(rand) % maze->height;
}

void add_path(struct maze *maze, int x, int y, RAND_TYPE *rand)
{
	static const struct direction {
		int dx, dy;
		TILE_TYPE here_mask, there_mask;
	} dirs[4] = {
		{ +1,  0, BIT_RIGHT, BIT_LEFT  },
		{  0, -1, BIT_UP   , BIT_DOWN  },
		{ -1,  0, BIT_LEFT , BIT_RIGHT },
		{  0, +1, BIT_DOWN , BIT_UP    }
	};

	TILE_TYPE *here = &MAZE_GET(maze, x, y);

	size_t around[4];
	size_t n_around = 0;
	size_t i;
	for (i = 0; i < 4; ++i) {
		const struct direction *d = &dirs[i];
		int tx = x + d->dx, ty = y + d->dy;
		if (coords_are_ok(maze, tx, ty)
		 && (MAZE_GET(maze, tx, ty) & BIT_PATH))
			around[n_around++] = i;
	}

	if (n_around > 0) {
		size_t j = (size_t)rand_gen(rand) % n_around;
		const struct direction *d = &dirs[around[j]];
		*here |= d->here_mask;
		MAZE_GET(maze, x + d->dx, y + d->dy) |= d->there_mask;
		if (n_around > 1 && rand_gen(rand) < LOOPINESS) {
			around[j] = around[--n_around];
			j = (size_t)rand_gen(rand) % n_around;
			d = &dirs[around[j]];
			*here |= d->here_mask;
			MAZE_GET(maze, x + d->dx, y + d->dy) |= d->there_mask;
		}
	}

	*here |= BIT_PATH;
}

int maze_generate(struct maze *maze, int width, int height, RAND_TYPE *rand)
{
	struct heads heads = { NULL, 0, 0 };
	int x, y;

	if (width <= 0 || height <= 0
	 || width > MAZE_MAX_WIDTH || height > MAZE_MAX_HEIGHT)
		goto error_dims;

	maze->width = width;
	maze->height = height;
	maze->tiles = calloc((size_t)(width * height), sizeof(*maze->tiles));
	if (!maze->tiles) goto error_maze_alloc;

	rand_point(maze, rand, &x, &y);
	add_path(maze, x, y, rand);
	if (add_heads(maze, x, y, &heads)) goto error_heads;

	while (heads.len > 0) {
		struct head *h =
			&heads.arr[(size_t)rand_gen(rand) % heads.len];
		add_path(maze, h->x, h->y, rand);
		if (add_heads(maze, h->x, h->y, &heads))
			goto error_heads;
		*h = heads.arr[--heads.len];
	}

	free(heads.arr);
	return 0;

error_heads:
	free(heads.arr);
	maze_destroy(maze);
error_maze_alloc:
error_dims:
	return -1;
}

void maze_destroy(struct maze *maze)
{
	free(maze->tiles);
}
