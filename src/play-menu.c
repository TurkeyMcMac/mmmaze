#include "play-menu.h"
#include "common-keys.h"
#include "game.h"
#include "help.h"
#include <curses.h>
#include <stdlib.h>
#include <time.h>

/* Prompts the user for numeric input in the given range. orig is the previous
 * value. Returned is the newly given value. A prompt is displayed for the
 * user. */
static unsigned long get_param(unsigned long orig,
	unsigned long min, unsigned long max)
{
	unsigned long param;
	int y, x;

	/* Show the cursor while getting input. */
	curs_set(1);

	/* Save the original position. */
	getyx(stdscr, y, x);
	printw("Enter an integer between %lu and %lu: ", min, max);
	/* Ask for input until valid input is given. */
	for (;;) {
		/* The maximum input length is 10 (buf is NUL-terminated): */
		char buf[11];
		/* The input length: */
		size_t len;
		/* The endptr for strtol: */
		char *endptr;
		/* Clear the input area. */
		clrtoeol();
		/* Get the input. */
		len = 0;
		for (;;) {
			int key = getch();
			switch (key) {
			/* Register only digits in input. */
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				if (len < sizeof(buf) - 1) {
					buf[len++] = key;
					/* Echo the input. */
					addch(key);
				}
				break;
			case '\n':
				/* The user pressed enter. */
				goto got_input;
			CASES_BACKSPACE
				if (len > 0) {
					/* Go back and erase. */
					addstr("\b \b");
					/* Remove the character. */
					--len;
				}
				break;
			CASES_QUIT
				/* If the user quits, the input is discarded and
				 * the previous parameter value is returned. */
				param = orig;
				goto ret;
			}
		}
	got_input:
		/* Add a NUL-terminator. */
		buf[len] = '\0';
		/* Parse the input. */
		param = strtoul(buf, &endptr, 10);
		if (*endptr == '\0' && endptr != buf
		 && param >= min && param <= max) {
			/* Return if the parameter parsed and is in range. */
			goto ret;
		} else {
			/* Otherwise, ask again. */
			mvprintw(y, x, "Please enter an integer between "
				"%lu and %lu: ", min, max);
		}
	}

ret:
	/* Clear the prompt. */
	move(y, x);
	clrtoeol();
	/* Hide the cursor again. */
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
		/* The default seed value is used for random seeds instead: */
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
			/* Again, the default seed value is used for random
			 * seeds instead: */
			if (params.seed == GAME_SEED_DEFAULT)
				params.seed = time(NULL);
			game_run(&params);
			/* Exit the parameter menu after the game ends. */
			return;
		case 'H':
			help_parameters_run();
			break;
		CASES_QUIT
			return;
		}
	}
}

