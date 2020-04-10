//
// Created by raphael on 17/02/20.
//

#ifndef BOMBERMAN_POSITION_H
#define BOMBERMAN_POSITION_H

typedef struct Position_t Position;

extern Position *Position_new(int x, int y);

extern void Position_free(Position *this);

extern int Position_get_X(Position *this);

extern void Position_set_X(Position *this, int x);

extern int Position_get_Y(Position *this);

extern void Position_set_Y(Position *this, int y);

#endif //BOMBERMAN_POSITION_H
