#include "maze.h"
#include "move.h"
#include "grow.h"
#include "other.h"
#include "player.h"
#include "rand.h"
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define VIEW_DIST 11

#define MAX_TRIES 10

#define MAX_CASH 20

#define OTHER_SPAWN_CHANCE (MAX_RAND / 30)

#define CASH_SPAWN_CHANCE (MAX_RAND / 10)

static int find_without_flags(const struct maze *maze, int *x, int *y,
	TILE_TYPE without, RAND_TYPE *rand)
{
	int tries = MAX_TRIES;
	while (tries-- > 0) {
		maze_random_node(maze, x, y, rand);
		if (!(MAZE_GET(maze, *x, *y) & without)) return 0;
	}
	return -1;
}

static chtype get_tile_char(TILE_TYPE tile)
{
	if (tile & BIT_OBSTRUCTED) {
		return 'O';
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

int main(void)
{
	struct maze maze;

	int end_x, end_y;

	int px = 0, py = 0;
	int cash_collected = 0;

	struct other *others = NULL;
	size_t n_others = 0;
	size_t others_cap = 0;

	int n_cash = 0;

	long ticks = 0;

	int key;

	RAND_TYPE seed, rand;

	initscr();
	noecho();
	curs_set(0);

	seed = time(NULL);
	rand = seed;

	maze_generate(&maze, 30, 30, &rand);

	end_x = maze.width - 1;
	end_y = maze.height - 1;

	player_mark_seen(&maze, px, py, VIEW_DIST);

	do {
		size_t i;
		int x, y;
		TILE_TYPE *pt, player_avail;
		int got_good_input = 0;

		pt = &MAZE_GET(&maze, px, py);
		if (*pt & BIT_CASH) {
			*pt &= ~BIT_CASH;
			++cash_collected;
			--n_cash;
		}

		player_avail = move_get_available(&maze, px, py);

		for (y = -VIEW_DIST; y <= VIEW_DIST; ++y) {
			for (x = -VIEW_DIST; x <= VIEW_DIST; ++x) {
				chtype ch = ' ';
				int mx = x + px, my = y + py;
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
				mvaddch(y + VIEW_DIST, x + VIEW_DIST, ch);
			}
		}
		mvprintw(0, VIEW_DIST * 2 + 1, "Cash collected: $%d",
			cash_collected);
		mvprintw(1, VIEW_DIST * 2 + 1, "Time: %ld", ticks);
		mvprintw(2, VIEW_DIST * 2 + 1, "Unclaimed cash: $%d", n_cash);
		mvprintw(3, VIEW_DIST * 2 + 1, "Others: %u",
			(unsigned)n_others);
		move(5, VIEW_DIST * 2 + 1);
		if (px == end_x && py == end_y) {
			addstr("Press SPACE to exit");
		} else if (player_avail == 0) {
			addstr("Press SPACE to advance");
		} else {
			addstr("Enter ? for help");
		}
		clrtoeol();
		mvprintw(7, VIEW_DIST * 2 + 1, "Seed: %lu",
			(long unsigned)seed);
		refresh();

		player_unmark_seen(&maze, px, py, VIEW_DIST);

		do {
			key = getch();
			switch (key) {
			case ' ':
				got_good_input = 1;
				break;
			case 'w':
				if (player_avail & BIT_UP) {
					got_good_input = 1;
					--py;
				}
				break;
			case 'a':
				if (player_avail & BIT_LEFT) {
					got_good_input = 1;
					--px;
				}
				break;
			case 's':
				if (player_avail & BIT_DOWN) {
					got_good_input = 1;
					++py;
				}
				break;
			case 'd':
				if (player_avail & BIT_RIGHT) {
					got_good_input = 1;
					++px;
				}
				break;
			case 'q':
				got_good_input = 1;
				break;
			}
		} while (!got_good_input);

		player_mark_seen(&maze, px, py, VIEW_DIST);

		for (i = 0; i < n_others; ++i) {
			other_start_move_random(&others[i], &maze, &rand);
		}

		if (rand_gen(&rand) < OTHER_SPAWN_CHANCE) {
			struct other *o = GROW(others, n_others, others_cap);
			int x, y;
			if (o
			 && find_without_flags(&maze, &x, &y,
					BIT_OBSTRUCTED | BIT_PLAYER_SEEN,
					&rand) >= 0)
				other_init(o, x, y, &maze);
		}

		if (n_cash < MAX_CASH) {
			int x, y;
			if (rand_gen(&rand) < CASH_SPAWN_CHANCE
			 && find_without_flags(&maze, &x, &y,
					BIT_CASH | BIT_PLAYER_SEEN, &rand) >= 0)
			{
				MAZE_GET(&maze, x, y) |= BIT_CASH;
				++n_cash;
			}
		}

		for (i = 0; i < n_others; ++i) {
			other_make_move(&others[i], &maze);
		}

		++ticks;
	} while (key != 'q');

	endwin();
	return EXIT_SUCCESS;
}

