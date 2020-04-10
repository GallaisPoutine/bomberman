//
// Created by raphael on 17/02/20.
//

#include "Position.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

struct Position_t {
    int x;
    int y;
};

extern Position *Position_new(int x, int y) {
    Position *this;
    this = (Position *) malloc(sizeof(Position));
    if (this == NULL) {
        fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Position));
        exit(EXIT_FAILURE);
    }

    this->x = x;
    this->y = y;

    return this;
}

extern void Position_free(Position *this) {
    assert(this != NULL);
    free(this);
}

extern int Position_get_X(Position *this) {
    return this->x;
}

extern void Position_set_X(Position *this, int x) {
    this->x = x;
}

extern int Position_get_Y(Position *this) {
    return this->y;
}

extern void Position_set_Y(Position *this, int y) {
    this->y = y;
}