//
// Created by raphael on 21/02/20.
//

#include "Field.h"

#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

// #include "Tile.h"

struct Field_t {
    int length;
    int depth;
    Tile ***grid;
};

static void Field_show_grid(Field *this) {
    for (int i=0; i<this->length; i++) {
        for (int j=0; i<this->depth; j++) {
            printf("%d %d %d\n", i, j, this->grid[i][j]== NULL? 0:1);
        }
    }
}

extern Field *Field_new(int length, int depth) {
    Field *this = (Field *) malloc(sizeof(Field));
    if (this == NULL) {
        fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Field));
        exit(EXIT_FAILURE);
    }

    this->length = length;
    this->depth = depth;

    // Allocation de la matrice de pointeurs
    this->grid = (Tile ***)calloc(depth*length, sizeof(Tile **));
    if (this->grid == NULL) {
        fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Tile **));
        exit(EXIT_FAILURE);
    }

    // Initialisation de chaque Tile dans la matrice
    for (int i=0; i<depth; i++) {
        this->grid[i] = (Tile **) calloc(length, sizeof(Tile *));
        if (this->grid[i] == NULL) {
            fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Tile *));
            exit(EXIT_FAILURE);
        }
        for (int j=0; j<length; j++) {
            // printf("New Tile creation %d %d\n", i, j);
            if (i == 0 || j == 0 || 
                i == depth -1 || j == length -1 ||
                (i%2 == 1 && j%2 == 1)) {
                this->grid[i][j] = Tile_new(WALL, i, j);
            } else {
                this->grid[i][j] = Tile_new(GROUND, i, j);
            }
        }
    }

    // Field_show_grid(this);
    return this;
}

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
    // TODO implement player in field
    // Check if he's in a wall
    Tile * tile = Field_get_tile(this, Player_get_X(player), Player_get_Y(player));
    assert(Tile_get_type(tile) == GROUND);
    if (Tile_get_type(tile) == GROUND) {
        Tile_add_player(tile, player);
    }
}
