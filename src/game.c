#include "game.h"
#include "common-keys.h"
#include "grow.h"
#include "help.h"
#include "monster.h"
#include "move.h"
#include "player.h"
#include <curses.h>

#define MAX_FIND_TRIES 10

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

	int end_x, end_y;

	int player_x = 0, player_y = 0;
	int cash_collected = 0;

	struct monster *monsters = NULL;
	size_t n_monsters = 0;
	size_t monsters_cap = 0;

	int n_cash = 0;

	long ticks = 0;

	RAND_TYPE rand = params->seed;

	erase();

	if (maze_generate(&maze, params->width, params->height, &rand)) {
		mvaddstr(0, 0, "The maze could not be created.\n\n(q) Quit");
		goto end_unconditionally;
	}

	end_x = maze.width - 1;
	end_y = maze.height - 1;

	player_mark_seen(&maze, player_x, player_y, params->view_dist);

	for (;;) {
		int do_tick = 0;
		TILE_TYPE player_avail;
		int info_offset;
		int x, y;

		player_avail = move_get_available(&maze, player_x, player_y);

		for (y = -params->view_dist; y <= params->view_dist; ++y) {
			for (x = -params->view_dist; x <= params->view_dist;
					++x) {
				chtype ch = ' ';
				int mx = x + player_x, my = y + player_y;
				if (mx >= 0 && my >= 0
				 && mx < maze.width && my < maze.height) {
					TILE_TYPE t = MAZE_GET(&maze, mx, my);
					if (t & BIT_PLAYER_SEEN) {
						if (x == 0 && y == 0) {
							ch = '@';
						} else if (mx == end_x
						 && my == end_y) {
							ch = A_STANDOUT | 'E';
						} else {
							ch = get_tile_char(t);
						}
					}
				}
				mvaddch(y + params->view_dist,
					x + params->view_dist, ch);
			}
		}

		info_offset = params->view_dist * 2 + 1;

		mvprintw(0, info_offset, "Cash collected: $%d", cash_collected);
		mvprintw(1, info_offset, "Time: %ld", ticks);
		mvprintw(2, info_offset, "Unclaimed cash: $%d", n_cash);
		mvprintw(3, info_offset, "Monsters: %u", (unsigned)n_monsters);
		mvprintw(4, info_offset, "Seed: %lu",
			(unsigned long)params->seed);

		move(6, info_offset);
		if (player_x == end_x && player_y == end_y) {
			addstr("(q) Finish maze");
		} else if (player_avail == 0) {
			addstr("(e) Advance one tick");
		} else {
			addstr("(H) Help");
		}
		clrtoeol();

		if (MAZE_GET(&maze, player_x, player_y) & BIT_MONSTER) {
			erase();
			mvaddstr(0, 0, "A monster got you.\n\n(q) Continue");
			goto end_unconditionally;
		}

		switch (getch()) {
		CASES_RIGHT
			if (player_avail & BIT_RIGHT) {
				player_unmark_seen(&maze, player_x, player_y,
					params->view_dist);
				++player_x;
				do_tick = 1;
			}
			break;
		CASES_UP
			if (player_avail & BIT_UP) {
				player_unmark_seen(&maze, player_x, player_y,
					params->view_dist);
				--player_y;
				do_tick = 1;
			}
			break;
		CASES_LEFT
			if (player_avail & BIT_LEFT) {
				player_unmark_seen(&maze, player_x, player_y,
					params->view_dist);
				do_tick = 1;
				--player_x;
			}
			break;
		CASES_DOWN
			if (player_avail & BIT_DOWN) {
				player_unmark_seen(&maze, player_x, player_y,
					params->view_dist);
				++player_y;
				do_tick = 1;
			}
			break;

		case 'e':
			do_tick = 1;
			break;

		case 'H':
			help_run();
			break;

		CASES_QUIT
			erase();
			if (player_x == end_x && player_y == end_y) {
				mvaddstr(0, 0, "You finished!\n");
				mvprintw(1, 0, "Cash collected: $%d",
					cash_collected);
				mvprintw(2, 0, "Time: %lu", ticks);
				mvprintw(3, 0, "Seed: %lu",
					(long unsigned)params->seed);
				mvaddstr(5, 0, "(q) Continue\n");
				goto end_unconditionally;
			} else {
				int key;
				mvaddstr(0, 0, "Quit the level?\n\n"
					"(y) Yes\n(n) No");
				do {
					key = getch();
					if (key == 'y') return;
				} while (key != 'n');
				do_tick = 0;
			}
			break;
		}

		if (do_tick) {
			size_t i;
			TILE_TYPE *pt;

			pt = &MAZE_GET(&maze, player_x, player_y);
			if (*pt & BIT_CASH) {
				*pt &= ~BIT_CASH;
				++cash_collected;
				--n_cash;
			}

			player_mark_seen(&maze, player_x, player_y,
				params->view_dist);

			for (i = 0; i < n_monsters; ++i) {
				monster_start_move_random(&monsters[i], &maze,
					&rand);
			}

			++ticks;

			if (ticks % params->monster_interval == 0) {
				struct monster *m = GROW(monsters, n_monsters,
					monsters_cap);
				int x, y;
				if (m && find_without_flags(&maze, &x, &y,
						BIT_MONSTER | BIT_PLAYER_SEEN,
						&rand) >= 0)
					monster_init(m, x, y, &maze);
			}

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

			for (i = 0; i < n_monsters; ++i) {
				monster_make_move(&monsters[i], &maze);
			}
		}
	}

end_unconditionally:
	for (;;) {
		switch (getch()) {
		CASES_QUIT
			return;
		}
	}
}
