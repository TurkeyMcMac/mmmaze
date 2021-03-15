#include "maze.h"
#include "grow.h"
#include <stdlib.h>

/* This means that if a node has an extra neighbor when it is placed, it has a
 * 1/10 chance to connect to that neighbor, forming a loop. I don't completely
 * understand this value's effects. */
#define LOOPINESS (MAX_RAND / 10)

/* The list of heads: places that will be later incorporated into the maze. */
struct heads {
	struct head {
		short x, y;
	} *arr;
	size_t len, cap;
};

/* Returns whether the coordinates are in the bounds of the maze. */
static int coords_are_ok(const struct maze *maze, int x, int y)
{
	return x >= 0 && y >= 0 && x < maze->width && y < maze->height;
}

/* Adds a head and marks its place as added, if the place isn't added yet. */
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

/* Adds up to four heads around the given place. */
static int add_heads(const struct maze *maze, int x, int y, struct heads *heads)
{
	if (add_head(maze, x + 2, y, heads)
	 || add_head(maze, x, y - 2, heads)
	 || add_head(maze, x - 2, y, heads)
	 || add_head(maze, x, y + 2, heads))
		return -1;
	return 0;
}

/* Incorporates the given place into the paths of the maze. */
static void add_path(struct maze *maze, int x, int y, RAND_TYPE *rand)
{
	/* The direction table. dx an dy are the relative offset from this
	 * place.here_mask is the bit added to the place here, and there_mask is
	 * the bit added to the place being connected to: */
	static const struct direction {
		int dx, dy;
		TILE_TYPE here_mask, there_mask;
	} dirs[4] = {
		{ +2,  0, BIT_RIGHT, BIT_LEFT  },
		{  0, -2, BIT_UP   , BIT_DOWN  },
		{ -2,  0, BIT_LEFT , BIT_RIGHT },
		{  0, +2, BIT_DOWN , BIT_UP    }
	};
	/* The tile here: */
	TILE_TYPE *here = &MAZE_GET(maze, x, y);
	/* The list of indices of available directions in which to expand: */
	size_t around[4];
	size_t n_around = 0;
	size_t i;
	for (i = 0; i < 4; ++i) {
		const struct direction *d = &dirs[i];
		int tx = x + d->dx, ty = y + d->dy;
		/* List directions going to places already added to the maze. */
		if (coords_are_ok(maze, tx, ty)
		 && (MAZE_GET(maze, tx, ty) & BIT_PATH))
			around[n_around++] = i;
	}

	/* Connect to the rest of the maze, if possible. */
	if (n_around > 0) {
		/* The index of a randomly chosen neighbor: */
		size_t j = (size_t)rand_gen(rand) % n_around;
		/* The randomly chosen direction: */
		const struct direction *d = &dirs[around[j]];
		/* Connect the paths. */
		*here |= d->here_mask;
		MAZE_GET(maze, x + d->dx, y + d->dy) |= d->there_mask;
		/* Add a loop, maybe. */
		if (n_around > 1 && rand_gen(rand) < LOOPINESS) {
			/* Do the same thing as above. */
			around[j] = around[--n_around];
			j = (size_t)rand_gen(rand) % n_around;
			d = &dirs[around[j]];
			*here |= d->here_mask;
			MAZE_GET(maze, x + d->dx, y + d->dy) |= d->there_mask;
		}
	}

	/* Mark the place as added to the maze. */
	*here |= BIT_PATH;
}

/* Removes bits which were only used in generation and must be reused later. */
static void remove_intermediate_bits(struct maze *maze)
{
	int x, y;
	for (y = 0; y < maze->height; y += 2) {
		for (x = 0; x < maze->width; x += 2) {
			MAZE_GET(maze, x, y) &= ~BIT_ADDED;
		}
	}
}

/* Connects the paths after they are all generated. Before this, the paths are
 * all spaced out on even coordinates only and are not truly connected. */
static void connect_paths(struct maze *maze)
{
	int x, y;
	for (y = 0; y < maze->height; y += 2) {
		for (x = 0; x < maze->width; x += 2) {
			TILE_TYPE t = MAZE_GET(maze, x, y);
			if (t & BIT_RIGHT)
				MAZE_GET(maze, x + 1, y) =
					BIT_PATH | BIT_LEFT | BIT_RIGHT;
			if (t & BIT_DOWN)
				MAZE_GET(maze, x, y + 1) =
					BIT_PATH | BIT_UP | BIT_DOWN;
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

	/* Convert the dimensions in nodes to dimensions in tiles. */
	maze->width = width_nodes * 2 - 1;
	maze->height = height_nodes * 2 - 1;
	maze->tiles = calloc((size_t)(maze->width * maze->height),
		sizeof(*maze->tiles));
	if (!maze->tiles) goto error_maze_alloc;

	/* Generate the first node. */
	maze_random_node(maze, &x, &y, rand);
	add_path(maze, x, y, rand);
	if (add_heads(maze, x, y, &heads)) goto error_heads;

	/* Fill in places until none are left. */
	while (heads.len > 0) {
		size_t i = (size_t)rand_gen(rand) % heads.len;
		add_path(maze, heads.arr[i].x, heads.arr[i].y, rand);
		if (add_heads(maze, heads.arr[i].x, heads.arr[i].y, &heads))
			goto error_heads;
		heads.arr[i] = heads.arr[--heads.len];
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

void maze_random_node(const struct maze *maze, int *x, int *y, RAND_TYPE *rand)
{
	/* Make sure the coordinates are even: */
	*x = (unsigned)rand_gen(rand) % ((maze->width + 1) / 2) * 2;
	*y = (unsigned)rand_gen(rand) % ((maze->height + 1) / 2) * 2;
}

void maze_destroy(struct maze *maze)
{
	free(maze->tiles);
}
