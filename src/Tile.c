//
// Created by raphael on 30/03/20.
//

#include "Tile.h"

#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

extern struct Tile *Tile_new(enum tile_type type, int x, int y) {
	struct Tile *tile = (struct Tile *)malloc(sizeof(struct Tile));
	if (!tile) {
		fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n",
			sizeof(struct Tile));
		exit(EXIT_FAILURE);
	}

	tile->has_bomb = false;
	tile->has_player = false;
	tile->type = type;
	tile->pos = Position_new(x, y);

	// TODO check tile
	tile->player = NULL;
	tile->bomb = NULL;

	return tile;
}

extern void Tile_free(struct Tile *tile) {
	assert(tile != NULL);
	if (!tile)
		return;

	Position_free(tile->pos);
	Player_free(tile->player);
	Bomb_free(tile->bomb);
	free(tile);
}

extern void Tile_add_player(struct Tile *tile, struct Player *p) {
	tile->player = p;
	tile->has_player = true;
}

extern void Tile_remove_player(struct Tile *tile) {
	if (tile->has_player) {
		tile->player = NULL;
		tile->has_player = false;
	}
}

extern void Tile_add_bomb(struct Tile *tile, struct Bomb *bomb) {
	tile->bomb = bomb;
	tile->has_bomb = true;
}

extern void Tile_remove_bomb(struct Tile *tile) {
	if (tile->has_bomb) {
		tile->bomb = NULL;
		tile->has_bomb = false;
	}
}

extern void Tile_destroy(struct Tile *tile) {
	if (tile->type == WOOD)
		tile->type = GROUND;
}

extern void Tile_fill(struct Tile *tile) {
	if (tile->type == GROUND)
		tile->type = WOOD;
}
