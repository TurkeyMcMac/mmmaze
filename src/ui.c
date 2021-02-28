#include "ui.h"
#include "help.h"
#include "play-menu.h"
#include <curses.h>

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
			play_menu_run();
			break;
		case 'H':
			help_run();
			break;
		case 'q':
			return;
		}
	}
}

void ui_run(void)
{
	initscr();
	noecho();
	curs_set(0);

	run_main_menu();

	endwin();
}
