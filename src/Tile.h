//
// Created by raphael on 30/03/20.
//

#ifndef BOMBERMAN_TILE_H
#define BOMBERMAN_TILE_H

#include <stdbool.h>

#include "Player.h"
#include "Position.h"

// typedef enum Tile_type_t Type;
typedef enum Tile_type_t{
    GROUND = 0,
    WATER,
    WALL,
} Type;

typedef struct Tile_t Tile;

extern Tile *Tile_new(Type type, int x, int y);

extern void Tile_free(Tile *this);

extern Position *Tile_get_position(Tile * this);

extern Type Tile_get_type(Tile *this);

extern bool Tile_has_player(Tile *this);

extern bool Tile_has_bomb(Tile *this);

extern void Tile_add_player(Tile *this, Player *player);

extern void Tile_remove_player(Tile *this);

#endif //BOMBERMAN_TIMER_H