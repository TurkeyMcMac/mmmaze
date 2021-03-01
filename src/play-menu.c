#include "play-menu.h"
#include "common-keys.h"
#include "game.h"
#include "help.h"
#include <curses.h>
#include <stdlib.h>
#include <time.h>

static unsigned long get_param(unsigned long orig,
	unsigned long min, unsigned long max)
{
	unsigned long param;
	int y, x;

	curs_set(1);

	getyx(stdscr, y, x);
	printw("Enter an integer between %lu and %lu: ", min, max);
	for (;;) {
		char buf[32];
		size_t len;
		char *endptr;
		clrtoeol();
		len = 0;
		for (;;) {
			int key = getch();
			switch (key) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				if (len < sizeof(buf) - 1) {
					buf[len++] = key;
					addch(key);
				}
				break;
			case '\n':
				goto got_input;
			CASES_BACKSPACE
				if (len > 0) {
					addstr("\b \b");
					--len;
				}
				break;
			CASES_QUIT
				param = orig;
				goto ret;
			}
		}
	got_input:
		buf[len] = '\0';
		param = strtoul(buf, &endptr, 10);
		if (*endptr == '\0' && endptr != buf
		 && param >= min && param <= max) {
			break;
		} else {
			mvprintw(y, x, "Please enter an integer between "
				"%lu and %lu: ", min, max);
		}
	}

ret:
	move(y, x);
	clrtoeol();
	curs_set(0);
	return param;
}

void play_menu_run(void)
{
	struct game_params params = GAME_DEFAULT_INITIALIZER;
	for (;;) {
		mvaddstr(0, 0, "Parameters\n\n");
		printw("(w) Width (nodes): %d\n", params.width);
		printw("(h) Height (nodes): %d\n", params.height);
		printw("(v) View distance: %d\n", params.view_dist);
		printw("(c) Cash placement interval: %d\n",
			params.cash_interval);
		printw("(C) Maximum placed cash: %d\n", params.max_cash);
		printw("(m) Monster placement interval: %d\n",
			params.monster_interval);
		printw("(s) Seed: %lu%s\n", (unsigned long)params.seed,
			params.seed == GAME_SEED_DEFAULT ? " (random)" : "");
		addstr(
			"\n(p) Play\n"
			"(H) Help\n"
			"(q) Cancel\n\n");
		clrtobot();

		switch (getch()) {
		case 'w':
			params.width = get_param(params.width,
				GAME_WIDTH_MIN, GAME_WIDTH_MAX);
			break;
		case 'h':
			params.height = get_param(params.height,
				GAME_HEIGHT_MIN, GAME_HEIGHT_MAX);
			break;
		case 'v':
			params.view_dist = get_param(params.view_dist,
				GAME_VIEW_DIST_MIN, GAME_VIEW_DIST_MAX);
			break;
		case 'c':
			params.cash_interval = get_param(params.cash_interval,
				GAME_CASH_INTERVAL_MIN, GAME_CASH_INTERVAL_MAX);
			break;
		case 'C':
			params.max_cash = get_param(params.max_cash,
				GAME_MAX_CASH_MIN, GAME_MAX_CASH_MAX);
			break;
		case 'm':
			params.monster_interval = get_param(
				params.monster_interval,
				GAME_MONSTER_INTERVAL_MIN,
				GAME_MONSTER_INTERVAL_MAX);
			break;
		case 's':
			params.seed = get_param(params.seed,
				GAME_SEED_MIN, GAME_SEED_MAX);
			break;

		case 'p':
			if (params.seed == GAME_SEED_DEFAULT)
				params.seed = time(NULL);
			game_run(&params);
			return;
		case 'H':
			help_parameters_run();
			break;
		CASES_QUIT
			return;
		}
	}
}

