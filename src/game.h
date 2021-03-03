#ifndef GAME_H_
#define GAME_H_

#include "maze.h"

/* See the in-game help for descriptions of the parameters. The maximum maximum
 * is 999999999 due to how parameters are input. */

#define GAME_WIDTH_DEFAULT 40
#define GAME_WIDTH_MIN 1
#define GAME_WIDTH_MAX MAZE_WIDTH_NODES_MAX

#define GAME_HEIGHT_DEFAULT 30
#define GAME_HEIGHT_MIN 1
#define GAME_HEIGHT_MAX MAZE_HEIGHT_NODES_MAX

#define GAME_VIEW_DIST_DEFAULT 11
#define GAME_VIEW_DIST_MIN 1
#define GAME_VIEW_DIST_MAX 1000

#define GAME_CASH_INTERVAL_DEFAULT 6
#define GAME_CASH_INTERVAL_MIN 1
#define GAME_CASH_INTERVAL_MAX 1000000UL

#define GAME_MAX_CASH_DEFAULT 50
#define GAME_MAX_CASH_MIN 0
#define GAME_MAX_CASH_MAX (GAME_WIDTH_MAX * GAME_HEIGHT_MAX)

#define GAME_MONSTER_INTERVAL_DEFAULT 18
#define GAME_MONSTER_INTERVAL_MIN 1
#define GAME_MONSTER_INTERVAL_MAX 1000000UL

#define GAME_SEED_DEFAULT 0
#define GAME_SEED_MIN 0
#define GAME_SEED_MAX 500000000UL

struct game_params {
	int width;
	int height;
	int view_dist;
	int cash_interval;
	int max_cash;
	int monster_interval;
	RAND_TYPE seed;
};

#define GAME_DEFAULT_INITIALIZER { \
	GAME_WIDTH_DEFAULT, \
	GAME_HEIGHT_DEFAULT, \
	GAME_VIEW_DIST_DEFAULT, \
	GAME_CASH_INTERVAL_DEFAULT, \
	GAME_MAX_CASH_DEFAULT, \
	GAME_MONSTER_INTERVAL_DEFAULT, \
	GAME_SEED_DEFAULT }

/* Run a game. This takes ownership of the screen but doesn't erase it when the
 * game ends. */
void game_run(const struct game_params *params);

#endif /* GAME_H_ */
