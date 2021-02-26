#ifndef TILE_H_
#define TILE_H_

/* TILE_TYPE currently holds eight bits. */
typedef unsigned char TILE_TYPE;

/* These bits identify which ways can be taken out of the tile. */
#define BIT_RIGHT 0x01
#define BIT_UP    0x02
#define BIT_LEFT  0x04
#define BIT_DOWN  0x08

/* These bits used while making the maze and are cleared afterward. They can be
 * reused later. */
/* The tile has been added to the list of heads. */
#define BIT_ADDED 0x10
/* A path has been placed in the tile. */
#define BIT_PATH  0x20

/* This bit shows that the player sees the tile. (Reused BIT_ADDED.) */
#define BIT_PLAYER_SEEN 0x10

/* This bit shows that someone is obstructing the tile. (Reused BIT_PATH.) */
#define BIT_OBSTRUCTED 0x20

/* This bit shows that there's cash in the tile. */
#define BIT_CASH 0x40

#endif /* TILE_H_ */
