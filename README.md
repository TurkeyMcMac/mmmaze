# Monster Money Maze

This is a simple game to be played on a text terminal. You find your way through
a maze, picking up cash before exiting at the bottom right. You must avoid
bumping into the monsters wandering randomly through the maze. The goal is to
leave with as much money as possible, but the longer you stay, the more monsters
appear, so be careful!

More information is available through the in-game menus.

## Compatibility

The code can be compiled with a C89 compiler and a Curses library. The code
works on Windows using PDCurses. To run the program, you should probably have a
terminal with dimensions of at least 80x23 that distinguishes uppercase and
lowercase letters.

## Building/Installation

### Unix

On Unix, you can compile `mmmaze` with `make`, or you can compile it manually:

    cc -o mmmaze -D_XOPEN_SOURCE src/*.c -lcurses

If you have Homebrew, you can also install the game like so:

    brew install turkeymcmac/tap/mmmaze

### Windows

You can download a pre-built Windows binary from the GitHub releases section.
It runs on the Windows Console specifically. My procedure to build it using
MingW and PDCurses is as follows:

    make -C ../PDCurses/wincon -j CC=x86_64-w64-mingw32-gcc CFLAGS='-I.. -std=c89 -Wall -Os -flto' pdcurses.a &&
    make exe=mmmaze.exe CC=x86_64-w64-mingw32-gcc CPPFLAGS='-I../PDCurses' CFLAGS='-std=c89 -pedantic -Wall -Wextra -Os -flto' LDFLAGS='-Wl,--gc-sections,--strip-all,--no-insert-timestamp' LDLIBS='../PDCurses/wincon/pdcurses.a'

(I keep the PDCurses root directory next to this project's root directory on my
computer, which is why the above references `../PDCurses`.)
