#include "ui.h"
#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	const char *progname = argv[0] ? argv[0] : "(no-name)";
	if (argc > 2) {
		fprintf(stderr, "%s: Too many arguments\n", progname);
		return EXIT_FAILURE;
	} else if (argc > 1) {
		if (!strcmp("-h", argv[1])) {
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
		} else if (!strcmp("-v", argv[1])) {
			printf("%s version " VERSION "\n", progname);
			return EXIT_SUCCESS;
		} else {
			fprintf(stderr, "%s: Invalid argument\n", progname);
			return EXIT_FAILURE;
		}
	} else {
		ui_run();
		return EXIT_SUCCESS;
	}
}
