//
// Created by raphael on 17/02/20.
//

#include "Player.h"

#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <pthread.h>

#include "Position.h"

struct Player_t {
    pthread_t player_thread;
    Position *pos;
};

extern Player *Player_new(int x, int y) {
    Player *this;
    this = (Player *) malloc(sizeof(Player));
    if (this == NULL) {
        fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Player));
        exit(EXIT_FAILURE);
    }

    this->pos = Position_new(x, y);

    return this;
}

extern void Player_free(Player *this) {
    assert(this != NULL);
    Position_free(this->pos);
    free(this);
}

extern int Player_get_X(Player *this) {
    return Position_get_X(this->pos);
}

extern void Player_set_X(Player *this, int x) {
    Position_set_X(this->pos, x);
}

extern int Player_get_Y(Player *this) {
    return Position_get_Y(this->pos);
}

extern void Player_set_Y(Player *this, int y) {
    Position_set_Y(this->pos, y);
}

extern void Player_move(Player *this, Move m) {
    switch (m) {
        case UP:
            Position_set_Y(this->pos, Player_get_Y(this) -1);
            break;
        case DOWN:
            Position_set_Y(this->pos, Player_get_Y(this) +1);
            break;
        case LEFT:
            Position_set_X(this->pos, Player_get_X(this) -1);
            break;
        case RIGHT:
            Position_set_X(this->pos, Player_get_X(this) +1);
            break;
        default:
            assert (m == NONE);
            break;
    }
}

extern Bomb *Player_pose_bomb(Player *this) {
    Bomb *bomb = Bomb_new(Player_get_X(this),
                        Player_get_Y(this));

    // Race condition when Player_free
    // while timer
    Bomb_start_timer(bomb);
    return bomb;
}