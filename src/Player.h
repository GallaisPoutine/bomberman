//
// Created by raphael on 17/02/20.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H

#include "Bomb.h"

#include <pthread.h>

struct Player {
	pthread_t player_thread;
	struct Position *pos;
};

enum player_move {
	NONE = 0,
	UP,
	DOWN,
	LEFT,
	RIGHT
};


extern struct Player *Player_new(int x, int y);
extern void Player_free(struct Player *player);
extern void Player_move(struct Player *player, enum player_move m);
extern struct Bomb *Player_pose_bomb(struct Player *player);

#endif //BOMBERMAN_PLAYER_H
