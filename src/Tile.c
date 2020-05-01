//
// Created by raphael on 30/03/20.
//

#include "Tile.h"

#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

struct Tile_t{
    bool has_bomb;
    bool has_player;
    Type type;
    Position *pos;
    Player *player;
    Bomb *bomb;
};

extern Tile *Tile_new(Type type, int x, int y) {
    Tile *this = (Tile *)malloc(sizeof(Tile));
    if (this == NULL) {
        fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Tile));
        exit(EXIT_FAILURE);
    }

    this->has_bomb = false;
    this->has_player = false;
    this->type = type;
    this->pos = Position_new(x, y);

    // TODO check this 
    this->player = NULL;
    this->bomb = NULL;

    return this;
}

extern void Tile_free(Tile *this) {
    assert(this != NULL);
    Position_free(this->pos);
    free(this);
}

extern Position *Tile_get_position(Tile * this) {
    return this->pos;
}

extern Type Tile_get_type(Tile *this) {
    return this->type;
}

extern Bomb *Tile_get_bomb(Tile *this) {
    return this->bomb;
}

extern bool Tile_has_player(Tile *this) {
    return this->has_player;
}

extern bool Tile_has_bomb(Tile *this) {
    return this->has_bomb;
}

extern void Tile_add_player(Tile *this, Player *player) {
    this->player = player;
    this->has_player = true;
}

extern void Tile_remove_player(Tile *this) {
    if (this->has_player) {
        this->player = NULL;
        this->has_player = false;
    }
}

extern void Tile_add_bomb(Tile *this, Bomb *bomb) {
    this->bomb = bomb;
    this->has_bomb = true;
}

extern void Tile_remove_bomb(Tile *this) {
    if (this->has_bomb) {
        this->bomb = NULL;
        this->has_bomb = false;
    }
}