#ifndef MAZE_H_
#define MAZE_H_

#include "rand.h"

typedef unsigned char TILE_TYPE;

struct maze {
	int width, height;
	TILE_TYPE *tiles;
};

#define MAZE_WIDTH_NODES_MAX 90
#define MAZE_HEIGHT_NODES_MAX 90

int maze_generate(struct maze *maze, int width_nodes, int height_nodes,
	RAND_TYPE *rand);

#define MAZE_GET(maze, x, y) ((maze)->tiles[(y) * (maze)->width + (x)])

void maze_random_node(const struct maze *maze, int *x, int *y, RAND_TYPE *rand);

void maze_destroy(struct maze *maze);

#endif /* MAZE_H_ */
