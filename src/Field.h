//
// Created by raphael on 21/02/20.
//

#ifndef BOMBERMAN_FIELD_H
#define BOMBERMAN_FIELD_H

#include "Bomb.h"
#include "Player.h"
#include "Tile.h"

struct Field {
	int length;
	int depth;
	struct Tile ***grid;
};

#define Field_get_tile(field, x, y) field->grid[x][y]

extern struct Field *Field_new(int length, int depth);
extern void Field_free(struct Field *field);

extern void Field_add_player(struct Field *field, struct Player *player);
extern void Field_move_player(struct Field *field, struct Player *player,
			      enum player_move m);
extern void Field_bomb_has_been_planted(struct Field *field, struct Player *p);
extern void Field_bomb_explosion(struct Field *field);
extern void Field_fill(struct Field *field);

#endif //BOMBERMAN_FIELD_H
