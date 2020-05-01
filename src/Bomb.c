//
// Created by raphael on 17/02/20.
//

#include "Bomb.h"

#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

#include "Position.h"
#include "Queue.h"
#include "Timer.h"

struct Bomb_t {
    Position *pos;
    Timer *timer;
};

extern Bomb * Bomb_new(int x, int y) {
    Bomb * this;
    this = (Bomb *) malloc(sizeof(Bomb));
    if (this == NULL){
        fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Bomb));
        exit(EXIT_FAILURE);
    }

    this->pos = Position_new(x, y);
    return this;
}

extern void Bomb_free(Bomb *this) {
    assert(this != NULL);
    Position_free(this->pos);
    Timer_free(this->timer);
    free(this);
}

extern int Bomb_get_X(Bomb *this) {
    return Position_get_X(this->pos);
}

extern void Bomb_set_X(Bomb *this, int x) {
    Position_set_X(this->pos, x);
}

extern int Bomb_get_Y(Bomb *this) {
    return Position_get_Y(this->pos);
}

extern void Bomb_set_Y(Bomb *this, int y) {
    Position_set_Y(this->pos, y);
}

extern void Bomb_start_timer(Bomb *this) {
    this->timer = Timer_new(NOMINAL_BOMB_DELAY, (Timer_callback) Bomb_explode, this);
    Timer_start(this->timer);
}

// TODO For tnow this is clearly a stub
// Send event in BAL ??
extern void Bomb_explode(Bomb *this) {
    // TODO manage
    adapter_t msg = {.msg = EV_EXPLODE};

    Queue_send(msg.buffer);

    // TODO destroy things
    Bomb_free(this);
}
