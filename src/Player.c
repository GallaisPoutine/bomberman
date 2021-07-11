//
// Created by raphael on 17/02/20.
//

#include "Player.h"

#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

#include "Position.h"


extern struct Player *Player_new(int x, int y) {
	struct Player *player = (struct Player *) malloc(sizeof(struct Player));
	if (!player) {
		fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n",
		sizeof(struct Player));
		exit(EXIT_FAILURE);
	}

	player->pos = Position_new(x, y);
	return player;
}

extern void Player_free(struct Player *player) {
	assert(player != NULL);

	if (!player)
		return;

	Position_free(player->pos);
	free(player);
}

extern void Player_move(struct Player *player, enum player_move m) {
	switch (m) {
	case UP:
		player->pos->y--;
		break;
	case DOWN:
		player->pos->y++;
		break;
	case LEFT:
		player->pos->x--;
		break;
	case RIGHT:
		player->pos->x++;
		break;
	default:
		assert (m == NONE);
		break;
	}
}

extern struct Bomb *Player_pose_bomb(struct Player *player) {
	struct Bomb *bomb = Bomb_new(player->pos->x, player->pos->y);

	// Race condition when Player_free
	// while timer
	Bomb_start_timer(bomb);
	return bomb;
}
