#include "game.h"
#include "common-keys.h"
#include "grow.h"
#include "help.h"
#include "monster.h"
#include "move.h"
#include "player.h"
#include <curses.h>
#include <stdlib.h>

/* The maximum tries for finding a random tile before giving up. */
#define MAX_FIND_TRIES 10

/* Find a tile for which the given 'without' flags are all off. If a random tile
 * is found, the x and y are stored in the arguments and 0 is returned.
 * Otherwise, -1 is returned. */
static int find_without_flags(const struct maze *maze, int *x, int *y,
	TILE_TYPE without, RAND_TYPE *rand)
{
	int tries = MAX_FIND_TRIES;
	while (tries-- > 0) {
		maze_random_node(maze, x, y, rand);
		if (!(MAZE_GET(maze, *x, *y) & without)) return 0;
	}
	return -1;
}

/* Return the Curses character corresponding to the tile's bits. */
static chtype get_tile_char(TILE_TYPE tile)
{
	if (tile & BIT_MONSTER) {
		return 'M';
	} else if (tile & BIT_CASH) {
		return '$';
	} else {
		int r = tile & BIT_RIGHT;
		int u = tile & BIT_UP;
		int l = tile & BIT_LEFT;
		int d = tile & BIT_DOWN;
		if (!r && !u && !l &&  d) return ACS_VLINE;
		if (!r && !u &&  l && !d) return ACS_HLINE;
		if (!r && !u &&  l &&  d) return ACS_URCORNER;
		if (!r &&  u && !l && !d) return ACS_VLINE;
		if (!r &&  u && !l &&  d) return ACS_VLINE;
		if (!r &&  u &&  l && !d) return ACS_LRCORNER;
		if (!r &&  u &&  l &&  d) return ACS_RTEE;
		if ( r && !u && !l && !d) return ACS_HLINE;
		if ( r && !u && !l &&  d) return ACS_ULCORNER;
		if ( r && !u &&  l && !d) return ACS_HLINE;
		if ( r && !u &&  l &&  d) return ACS_TTEE;
		if ( r &&  u && !l && !d) return ACS_LLCORNER;
		if ( r &&  u && !l &&  d) return ACS_LTEE;
		if ( r &&  u &&  l && !d) return ACS_BTEE;
		if ( r &&  u &&  l &&  d) return ACS_PLUS;
	}
	return ' ';
}

void game_run(const struct game_params *params)
{
	struct maze maze;
	/* Whether the maze has been generated: */
	int maze_created = 0;
	/* The coordinates of the maze end at the bottom right: */
	int end_x, end_y;
	/* The player position: */
	int player_x = 0, player_y = 0;
	/* The player's collected cash: */
	int cash_collected = 0;
	/* The list of monsters, its length, and its allocation capacity: */
	struct monster *monsters = NULL;
	size_t n_monsters = 0;
	size_t monsters_cap = 0;
	/* The amount of cash currently in the maze: */
	int n_cash = 0;
	/* The number of ticks passed: */
	long ticks = 0;
	/* The random state: */
	RAND_TYPE rand = params->seed;

	erase();

	if (maze_generate(&maze, params->width, params->height, &rand)) {
		mvaddstr(0, 0, "The maze could not be created.\n\n(q) Quit");
		goto end_with_prompt;
	}
	/* After generation, mark the maze as such. */
	maze_created = 1;

	/* Initialize the end place to the bottom right. */
	end_x = maze.width - 1;
	end_y = maze.height - 1;

	/* Mark what's visible for the first draw: */
	plyr_mark_seen(&maze, player_x, player_y, params->view_dist);

	for (;;) {
		/* Whether to simulate one tick this iteration: */
		int do_tick = 0;
		/* The direction bits available to the player on their tile: */
		TILE_TYPE player_avail;
		/* The x offset of the info on the screen: */
		int info_offset;
		/* Coordinates relative to the player, for iteration: */
		int x, y;

		/* Make the player lose if a monster is upon them. */
		if (MAZE_GET(&maze, player_x, player_y) & BIT_MONSTER) {
			mvaddstr(0, 0, "A monster got you.\n\n(q) Continue");
			goto end_with_prompt;
		}

		/* Calculate available directions: */
		player_avail = move_get_available(&maze, player_x, player_y);
		/* Draw the tiles that could be visible to the player: */
		for (y = -params->view_dist; y <= params->view_dist; ++y) {
			for (x = -params->view_dist; x <= params->view_dist;
					++x) {
				chtype ch = ' ';
				/* The coordinates within the maze: */
				int mx = x + player_x, my = y + player_y;
				/* Only try to draw in-range coordinates: */
				if (mx >= 0 && my >= 0
				 && mx < maze.width && my < maze.height) {
					TILE_TYPE t = MAZE_GET(&maze, mx, my);
					if (t & BIT_PLAYER_SEEN) {
						if (x == 0 && y == 0) {
							/* Draw the player. */
							ch = '@';
						} else if (mx == end_x
						 && my == end_y) {
							/* Draw the end. */
							ch = A_STANDOUT | 'E';
						} else {
							/* Draw the tile. */
							ch = get_tile_char(t);
						}
					}
				}
				/* Actually draw the pixel. */
				mvaddch(y + params->view_dist,
					x + params->view_dist, ch);
			}
		}

		/* The info is to the right of the maze view. */
		info_offset = params->view_dist * 2 + 1;
		/* Draw the numeric info. */
		mvprintw(0, info_offset, "Cash collected: $%d", cash_collected);
		mvprintw(1, info_offset, "Time: %ld", ticks);
		mvprintw(2, info_offset, "Unclaimed cash: $%d", n_cash);
		mvprintw(3, info_offset, "Monsters: %u", (unsigned)n_monsters);
		mvprintw(4, info_offset, "Seed: %lu",
			(unsigned long)params->seed);
		/* Draw the hint. */
		move(6, info_offset);
		if (player_x == end_x && player_y == end_y) {
			/* Hint that the player can leave at the end. */
			addstr("(q) Finish maze");
		} else if (player_avail == 0) {
			/* Hint that the player can wait when they are stuck. */
			addstr("(i) Advance one tick");
		} else {
			/* Tell the player how to get help. */
			addstr("(?) Help");
		}
		clrtoeol();

		/* Otherwise, get player input. */
		switch (getch()) {
		CASES_RIGHT
			if (player_avail & BIT_RIGHT) {
				plyr_unmark_seen(&maze, player_x, player_y,
					params->view_dist);
				++player_x;
				do_tick = 1;
			}
			break;
		CASES_UP
			if (player_avail & BIT_UP) {
				plyr_unmark_seen(&maze, player_x, player_y,
					params->view_dist);
				--player_y;
				do_tick = 1;
			}
			break;
		CASES_LEFT
			if (player_avail & BIT_LEFT) {
				plyr_unmark_seen(&maze, player_x, player_y,
					params->view_dist);
				--player_x;
				do_tick = 1;
			}
			break;
		CASES_DOWN
			if (player_avail & BIT_DOWN) {
				plyr_unmark_seen(&maze, player_x, player_y,
					params->view_dist);
				++player_y;
				do_tick = 1;
			}
			break;

		case 'i':
			/* Idle. */
			do_tick = 1;
			break;

		case '?':
			help_run();
			erase();
			break;

		CASES_QUIT
			if (player_x == end_x && player_y == end_y) {
				/* The player quits and completes the game. */
				mvprintw(0, 0,
					"You finished!\n"
					"Cash collected: $%d\n"
					"Time: %lu\n"
					"Seed: %lu\n\n"
					"(q) Continue", cash_collected, ticks,
					(unsigned long)params->seed);
				goto end_with_prompt;
			} else {
				/* The player quits in another situation. */
				int key;
				/* In this case, ask for confirmation first. */
				mvaddstr(0, 0, "Quit the level?\n\n"
					"(y) Yes\n(n) No");
				clrtobot();
				do {
					key = getch();
					if (key == 'y') goto end;
				} while (key != 'n');
			}
			break;
		}

		if (do_tick) {
			/* For iteration: */
			size_t i;
			/* The tile the player is on: */
			TILE_TYPE *pt;

			/* Collect cash if possible. */
			pt = &MAZE_GET(&maze, player_x, player_y);
			if (*pt & BIT_CASH) {
				*pt &= ~BIT_CASH;
				++cash_collected;
				--n_cash;
			}

			/* Mark the new seen area. */
			plyr_mark_seen(&maze, player_x, player_y,
				params->view_dist);

			/* Calculate monster movement. */
			for (i = 0; i < n_monsters; ++i) {
				mnst_start_move_random(&monsters[i], &maze,
					&rand);
			}

			/* Advance a tick for upcoming placement intervals. */
			++ticks;

			/* Possibly place a monster. */
			if (ticks % params->monster_interval == 0) {
				struct monster *m = GROW(monsters, n_monsters,
					monsters_cap);
				int x, y;
				if (m && find_without_flags(&maze, &x, &y,
						BIT_MONSTER | BIT_PLAYER_SEEN,
						&rand) >= 0)
					mnst_init(m, x, y, &maze);
			}

			/* Possibly place $1. */
			if (n_cash < params->max_cash
			 && ticks % params->cash_interval == 0) {
				int x, y;
				if (find_without_flags(&maze, &x, &y, BIT_CASH |
						BIT_PLAYER_SEEN, &rand) >= 0)
				{
					MAZE_GET(&maze, x, y) |= BIT_CASH;
					++n_cash;
				}
			}

			/* Carry out monster movement. */
			for (i = 0; i < n_monsters; ++i) {
				mnst_make_move(&monsters[i], &maze);
			}
		}
	}

end_with_prompt:
	/* Finish drawing the prompt. */
	clrtobot();
	/* End the game and make sure the player sees something first. */
	for (;;) {
		switch (getch()) {
		CASES_QUIT
			goto end;
		}
	}
end:
	/* Release memory. */
	free(monsters);
	if (maze_created) maze_destroy(&maze);
}
