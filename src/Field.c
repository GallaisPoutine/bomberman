//
// Created by raphael on 21/02/20.
//

#include "Field.h"

#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

#include "Queue.h"

struct Field_t {
    int length;
    int depth;
    Tile ***grid;
};

extern Field *Field_new(int length, int depth) {
    Field *this = (Field *) malloc(sizeof(Field));
    if (this == NULL) {
        fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Field));
        exit(EXIT_FAILURE);
    }

    this->length = length;
    this->depth = depth;

    // Allocation de la matrice de pointeurs
    this->grid = (Tile ***)calloc(length, sizeof(Tile **));
    if (this->grid == NULL) {
        fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Tile **));
        exit(EXIT_FAILURE);
    }

    // Initialisation de chaque Tile dans la matrice
    for (int i=0; i<length; i++) {
        this->grid[i] = (Tile **) calloc(depth, sizeof(Tile *));
        if (this->grid[i] == NULL) {
            fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Tile *));
            exit(EXIT_FAILURE);
        }
        for (int j=0; j<depth; j++) {
            if (i == 0 || j == 0 || 
                i == length -1 || j == depth -1 ||
                (i%2 == 1 && j%2 == 1)) {
                this->grid[i][j] = Tile_new(WALL, i, j);
            } else {
                this->grid[i][j] = Tile_new(GROUND, i, j);
            }
        }
    }

    return this;
}

// TODO memory leaks around here
extern void Field_free(Field *this) {
    assert(this != NULL);
    for (int i=0; i<this->length; i++) {
        assert(this->grid[i] != NULL);
        free(this->grid[i]);
        // for (int j=0; i<this->depth; j++) {
        //     assert(this->grid[i][j] != NULL);
        //     free(this->grid[i][j]);
        // }
    }
    free(this);
}

extern Tile ***Field_get_grid(Field *this) {
    return this->grid;
}

extern Tile *Field_get_tile(Field *this, int x, int y) {
    return this->grid[x][y];
}

extern int Field_get_length(Field *this) {
    return this->length;
}

extern int Field_get_depth(Field *this) {
    return this->depth;
}

extern void Field_add_player(Field *this, Player *player){
    printf("Player Added\n");
    // TODO check if he's in a wall
    Tile * tile = Field_get_tile(this, Player_get_X(player), Player_get_Y(player));
    assert(Tile_get_type(tile) == GROUND);
    if (Tile_get_type(tile) == GROUND) {
        Tile_add_player(tile, player);
    }
}

// TODO optimize switch case
extern void Field_move_player(Field *this, Player *player, Move m) {
    int x = Player_get_X(player);
    int y = Player_get_Y(player);

    switch (m) {
    case UP:
        y = Player_get_Y(player) -1;
        break;
    case DOWN:
        y = Player_get_Y(player) +1;
        break;
    case LEFT:
        x = Player_get_X(player) -1;
        break;
    case RIGHT:
        x = Player_get_X(player) +1;
        break;
    default:
        assert (m == NONE);
        break;
    }

    Tile *tile = Field_get_tile(this, Player_get_X(player), Player_get_Y(player));
    Tile *tile_next = Field_get_tile(this, x, y);

    if (Tile_get_type(tile_next) == GROUND) {
        Tile_remove_player(tile);
        Tile_add_player(tile_next, player);
        Player_move(player, m);
    }
}

extern void Field_bomb_has_been_planted(Field *this, Player *p) {
    bomb_adapter_t b = {.bomb = Player_pose_bomb(p)};
    Tile *t = Field_get_tile(this, Bomb_get_X(b.bomb), Bomb_get_Y(b.bomb));
    assert(Tile_get_type(t) == GROUND);
    Tile_add_bomb(t, b.bomb);
    Queue_send(MQ_BOMBS_NAME, b.buffer);
}

extern void Field_bomb_explosion(Field *this) {
    bomb_adapter_t b = {.buffer = 0};
    Queue_receive(MQ_BOMBS_NAME, b.buffer);
    int x = Bomb_get_X(b.bomb);
    int y = Bomb_get_Y(b.bomb);

    for (int i=x -BOMB_INTENSITY; i<=x +BOMB_INTENSITY; i++) {
        Tile *ttmp = Field_get_tile(this, i, y);
        if (Tile_get_type(ttmp) != WALL && Tile_has_player(ttmp)) {
            Tile_remove_player(ttmp);
        }
    }

    for (int j=y -BOMB_INTENSITY; j<=y +BOMB_INTENSITY; j++) {
        Tile *ttmp = Field_get_tile(this, x, j);
        if (Tile_get_type(ttmp) != WALL && Tile_has_player(ttmp)) {
            Tile_remove_player(ttmp);
        }
    }

    Tile_remove_bomb(Field_get_tile(this, x, y));
    // Bomb_free(b.bomb);
}