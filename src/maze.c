#include "maze.h"
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
		if (!(*t & BIT_ADDED)) {
			struct head *h =
				GROW(heads->arr, heads->len, heads->cap);
			if (!h) return -1;
			h->x = x;
			h->y = y;
			*t |= BIT_ADDED;
		}
	}
	return 0;
}

static int add_heads(const struct maze *maze, int x, int y, struct heads *heads)
{
	if (add_head(maze, x + 2, y, heads)
	 || add_head(maze, x, y - 2, heads)
	 || add_head(maze, x - 2, y, heads)
	 || add_head(maze, x, y + 2, heads))
		return -1;
	return 0;
}

void maze_random_node(const struct maze *maze, int *x, int *y, RAND_TYPE *rand)
{
	*x = (unsigned)rand_gen(rand) % ((maze->width + 1) / 2) * 2;
	*y = (unsigned)rand_gen(rand) % ((maze->height + 1) / 2) * 2;
}

void add_path(struct maze *maze, int x, int y, RAND_TYPE *rand)
{
	static const struct direction {
		int dx, dy;
		TILE_TYPE here_mask, there_mask;
	} dirs[4] = {
		{ +2,  0, BIT_RIGHT, BIT_LEFT  },
		{  0, -2, BIT_UP   , BIT_DOWN  },
		{ -2,  0, BIT_LEFT , BIT_RIGHT },
		{  0, +2, BIT_DOWN , BIT_UP    }
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

static void remove_intermediate_bits(struct maze *maze)
{
	int x, y;
	for (y = 0; y < maze->height; y += 2) {
		for (x = 0; x < maze->width; x += 2) {
			MAZE_GET(maze, x, y) &= ~(BIT_ADDED | BIT_PATH);
		}
	}
}

static void connect_paths(struct maze *maze)
{
	int x, y;
	for (y = 0; y < maze->height; y += 2) {
		for (x = 0; x < maze->width; x += 2) {
			TILE_TYPE t = MAZE_GET(maze, x, y);
			if (t & BIT_RIGHT)
				MAZE_GET(maze, x + 1, y) = BIT_LEFT | BIT_RIGHT;
			if (t & BIT_DOWN)
				MAZE_GET(maze, x, y + 1) = BIT_UP | BIT_DOWN;
		}
	}
}

int maze_generate(struct maze *maze, int width_nodes, int height_nodes,
	RAND_TYPE *rand)
{
	struct heads heads = { NULL, 0, 0 };
	int x, y;

	if (width_nodes <= 0 || height_nodes <= 0
	 || width_nodes > MAZE_WIDTH_NODES_MAX
	 || height_nodes > MAZE_HEIGHT_NODES_MAX)
		goto error_dims;

	maze->width = width_nodes * 2 - 1;
	maze->height = height_nodes * 2 - 1;
	maze->tiles = calloc((size_t)(maze->width * maze->height),
		sizeof(*maze->tiles));
	if (!maze->tiles) goto error_maze_alloc;

	maze_random_node(maze, &x, &y, rand);
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

	remove_intermediate_bits(maze);

	connect_paths(maze);

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
