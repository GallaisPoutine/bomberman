//
// Created by raphael on 21/02/20.
//

#ifndef BOMBERMAN_FIELD_H
#define BOMBERMAN_FIELD_H

#include "Player.h"
#include "Tile.h"

typedef struct Field_t Field;

extern Field *Field_new(int length, int depth);

extern void Field_free(Field *this);

extern Tile ***Field_get_grid(Field *this);

extern Tile *Field_get_tile(Field *this, int x, int y);

extern int Field_get_length(Field *this);

extern int Field_get_depth(Field *this);

extern void Field_add_player(Field *this, Player *player);

#endif //BOMBERMAN_FIELD_H
