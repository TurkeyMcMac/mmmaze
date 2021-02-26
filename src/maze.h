#ifndef MAZE_H_
#define MAZE_H_

#include "rand.h"

typedef short TILE_TYPE;

struct maze {
	int width, height;
	TILE_TYPE *tiles;
};

#define MAZE_MAX_WIDTH 181
#define MAZE_MAX_HEIGHT 181

int maze_generate(struct maze *maze, int width, int height, RAND_TYPE *rand);

#define MAZE_GET(maze, x, y) ((maze)->tiles[(y) * (maze)->width + (x)])

void maze_destroy(struct maze *maze);

#endif /* MAZE_H_ */
