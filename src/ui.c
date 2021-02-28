#include "ui.h"
#include "game.h"
#include "help.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void run_main_menu(void);
static void run_play_menu(void);

void ui_run(void)
{
	initscr();
	noecho();
	curs_set(0);

	run_main_menu();

	endwin();
}

static void run_main_menu(void)
{
	for (;;) {
		mvaddstr(0, 0,
			"Monster Money Maze\n\n"
			"(p) Play\n"
			"(H) Help\n"
			"(q) Quit");
		clrtobot();

		switch (getch()) {
		case 'p':
			run_play_menu();
			break;
		case 'H':
			help_run();
			break;
		case 'q':
			return;
		}
	}
}

static unsigned long get_param(unsigned long min, unsigned long max)
{
	unsigned long param;
	int y, x;
	char buf[32];

	curs_set(1);
	echo();

	getyx(stdscr, y, x);
	(void)x;
	printw("Enter an integer between %lu and %lu: ", min, max);
	for (;;) {
		char *endptr;
		clrtoeol();
		memset(buf, '\0', sizeof(buf));
		getnstr(buf, sizeof(buf) - 1);
		move(y, 0);
		param = strtoul(buf, &endptr, 10);
		if (*endptr == '\0' && endptr != buf
		 && param >= min && param <= max) {
			clrtoeol();
			break;
		} else {
			printw("Please enter an integer between "
				"%lu and %lu: ", min, max);
		}
	}

	noecho();
	curs_set(0);

	return param;
}

static void run_play_menu(void)
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
			params.width =
				get_param(GAME_WIDTH_MIN, GAME_WIDTH_MAX);
			break;
		case 'h':
			params.height =
				get_param(GAME_HEIGHT_MIN, GAME_HEIGHT_MAX);
			break;
		case 'v':
			params.view_dist = get_param(GAME_VIEW_DIST_MIN,
				GAME_VIEW_DIST_MAX);
			break;
		case 'c':
			params.cash_interval = get_param(GAME_CASH_INTERVAL_MIN,
				GAME_CASH_INTERVAL_MAX);
			break;
		case 'C':
			params.max_cash = get_param(GAME_MAX_CASH_MIN,
				GAME_MAX_CASH_MAX);
			break;
		case 'm':
			params.monster_interval = get_param(
				GAME_MONSTER_INTERVAL_MIN,
				GAME_MONSTER_INTERVAL_MAX);
			break;
		case 's':
			params.seed = get_param(GAME_SEED_MIN, GAME_SEED_MAX);
			break;

		case 'p':
			if (params.seed == GAME_SEED_DEFAULT)
				params.seed = time(NULL);
			game_run(&params);
			return;
		case 'H':
			help_parameters_run();
			break;
		case 'q':
			return;
		}
	}
}
