#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.2.0"

int main(int argc, char *argv[])
{
	const char *progname = argv[0] ? argv[0] : "(no-name)";
	int i;
	for (i = 1; i < argc; ++i) {
		if (!strcmp("-h", argv[i])) {
			printf(
				"Usage: %s [{-h | -v}]\n"
				"Plays the game Monster Money Maze.\n"
				"\n"
				"Options:\n"
				"  -h Print this help rather than playing.\n"
				"  -v Print the version rather than playing.\n"
				"\n"
				"See the in-game help information for more.\n",
				progname);
			return EXIT_SUCCESS;
		} else if (!strcmp("-v", argv[i])) {
			printf("%s version " VERSION "\n", progname);
			return EXIT_SUCCESS;
		} else {
			fprintf(stderr, "%s: Invalid argument: %s\n",
				progname, argv[i]);
			return EXIT_FAILURE;
		}
	}

	ui_run();

	return EXIT_SUCCESS;
}
