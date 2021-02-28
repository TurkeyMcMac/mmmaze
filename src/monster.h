#ifndef MONSTER_H_
#define MONSTER_H_

#include "maze.h"

struct monster {
	short x, y;
	TILE_TYPE back_dir;
	TILE_TYPE next_move;
};

void monster_init(struct monster *monster, int x, int y, struct maze *maze);

void monster_start_move_random(struct monster *monster, const struct maze *maze,
	RAND_TYPE *rand);

void monster_make_move(struct monster *monster, struct maze *maze);

#endif /* MONSTER_H_ */
