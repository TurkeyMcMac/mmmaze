#ifndef MONSTER_H_
#define MONSTER_H_

#include "maze.h"

/* A maze monster. */
struct monster {
	short x, y;
	/* The tile direction bit kept so that the monster doesn't randomly turn
	 * around 180 degrees. If this is 0, the monster can move any way. */
	TILE_TYPE back_dir;
	/* The direction in which the monster plans to move, or 0 if it plans on
	 * not moving. */
	TILE_TYPE next_move;
};

/* Creates a monster, initializing it in the structure and the maze. It
 * initially has no plan and can move any direction. */
void monster_init(struct monster *monster, int x, int y, struct maze *maze);

/* Have the monster plan its next move randomly. */
void monster_start_move_random(struct monster *monster, const struct maze *maze,
	RAND_TYPE *rand);

/* Carry out the monster's move. It will not move if its way is blocked by
 * another monster. */
void monster_make_move(struct monster *monster, struct maze *maze);

#endif /* MONSTER_H_ */
