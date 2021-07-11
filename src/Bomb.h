//
// Created by raphael on 17/02/20.
//

#ifndef BOMBERMAN_BOMB_H
#define BOMBERMAN_BOMB_H

#define BOMB_INTENSITY (3)

struct Bomb {
	struct Position *pos;
	struct Timer *timer;
};

extern struct Bomb *Bomb_new(int x, int y);
extern void Bomb_free(struct Bomb *bomb);
extern void Bomb_start_timer(struct Bomb *bomb);
extern void Bomb_explode(struct Bomb *bomb);

#endif //BOMBERMAN_BOMB_H
