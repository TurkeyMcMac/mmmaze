#ifndef MOVE_H_
#define MOVE_H_

#include "maze.h"

/* Gets the tile direction bits which an entity can move at the given position. 
 * Monsters block movement. */
TILE_TYPE move_get_available(const struct maze *maze, int x, int y);

#endif /* MOVE_H_ */
