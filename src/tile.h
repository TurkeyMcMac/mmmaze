#ifndef TILE_H_
#define TILE_H_

/* TILE_TYPE currently holds eight bits. */
typedef unsigned char TILE_TYPE;

/* These bits identify which ways can be taken out of the tile. */
#define BIT_RIGHT 0x01
#define BIT_UP    0x02
#define BIT_LEFT  0x04
#define BIT_DOWN  0x08

/* A path has been placed in the tile. */
#define BIT_PATH  0x10

/* The tile has been added to the list of heads. This bit is used while making
 * the maze and is cleared afterward. It can be reused later.*/
#define BIT_ADDED 0x20

/* This bit shows that the player sees the tile. (Reused BIT_ADDED.) */
#define BIT_PLAYER_SEEN 0x20

/* This bit shows that a monster is obstructing the tile. */
#define BIT_MONSTER 0x40

/* This bit shows that there's cash in the tile. */
#define BIT_CASH 0x80

#endif /* TILE_H_ */
