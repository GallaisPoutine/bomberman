//
// Created by raphael on 17/02/20.
//

#ifndef BOMBERMAN_POSITION_H
#define BOMBERMAN_POSITION_H

struct Position {
	int x;
	int y;
};

extern struct Position *Position_new(int x, int y);
extern void Position_free(struct Position *pos);

#endif //BOMBERMAN_POSITION_H
