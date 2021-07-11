//
// Created by raphael on 30/03/20.
//

#ifndef BOMBERMAN_TILE_H
#define BOMBERMAN_TILE_H

#include <stdbool.h>

#include "Player.h"
#include "Position.h"

// typedef enum Tile_type_t Type;
enum tile_type {
	GROUND = 0,	// DEFAULT TILE TYPE
	WATER,		// CAN'T WALK ON IT
	WALL,		// NOT DESTROYABLE
	WOOD		// DESTROYABLE MATERIAL
};

struct Tile {
	bool has_bomb;
	bool has_player;
	enum tile_type type;
	struct Position *pos;
	struct Player *player;
	struct Bomb *bomb;
};

extern struct Tile *Tile_new(enum tile_type type, int x, int y);
extern void Tile_free(struct Tile *tile);

extern void Tile_add_player(struct Tile *tile, struct Player *player);
extern void Tile_remove_player(struct Tile *tile);
extern void Tile_add_bomb(struct Tile *tile, struct Bomb *bomb);
extern void Tile_remove_bomb(struct Tile *tile);
extern void Tile_destroy(struct Tile *tile);
extern void Tile_fill(struct Tile *tile);

#endif //BOMBERMAN_TIMER_H
