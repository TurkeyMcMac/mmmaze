#ifndef MAZE_H_
#define MAZE_H_

#include "rand.h"
#include "tile.h"

/* The maze structure. The maze is made of nodes placed at even coordinates, all
 * of which are part of the maze paths. The tiles between nodes are just for
 * connecting them. There will be some loops. The width and height will be odd,
 * and consequently the bottom right tile will be a node just as the top left
 * will be. Going from the top left to the bottom right, following the path
 * directions specified by the tile bits will never lead one to out-of-bounds
 * coordinates, and all nodes will be reachable along the way. All tiles part of
 * the path are marked with BIT_PATH. */
struct maze {
	int width, height;
	TILE_TYPE *tiles;
};

/* The maximum width and height in nodes, chosen so that width * height (of type
 * int) can never overflow, and so that x and y coordinates can each fit inside
 * one byte. */
#define MAZE_WIDTH_NODES_MAX 90
#define MAZE_HEIGHT_NODES_MAX 90

/* Generates a maze with the random state. The width and height in nodes must be
 * positive and are also constrained by the constants above. Returns 0 on
 * success or -1 on allocation failure/invalid arguments. */
int maze_generate(struct maze *maze, int width_nodes, int height_nodes,
	RAND_TYPE *rand);

/* Gets the lvalue of a tile without checking bounds. */
#define MAZE_GET(maze, x, y) ((maze)->tiles[(y) * (maze)->width + (x)])

/* Places the coordinates in x and y of a random node, i.e., a tile which will
 * always be part of the maze paths. */
void maze_random_node(const struct maze *maze, int *x, int *y, RAND_TYPE *rand);

/* Deallocates the maze resources. */
void maze_destroy(struct maze *maze);

#endif /* MAZE_H_ */
