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


extern struct Bomb * Bomb_new(int x, int y) {
	struct Bomb * bomb = (struct Bomb *) malloc(sizeof(struct Bomb));
	if (!bomb){
		fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n",
			sizeof(struct Bomb));
		exit(EXIT_FAILURE);
	}

	bomb->pos = Position_new(x, y);
	return bomb;
}

extern void Bomb_free(struct Bomb *bomb) {
	assert(bomb != NULL);
	if (!bomb)
		return;

	Position_free(bomb->pos);
	Timer_free(bomb->timer);
	free(bomb);
}

extern void Bomb_start_timer(struct Bomb *bomb) {
	bomb->timer = Timer_new(NOMINAL_BOMB_DELAY,
				(Timer_callback) Bomb_explode, bomb);
	Timer_start(bomb->timer);
}

// TODO For tnow bomb is clearly a stub
// Send event in BAL ??
extern void Bomb_explode(struct Bomb *bomb) {
	// TODO manage
	adapter_t msg = {.msg = EV_EXPLODE};

	Queue_send(MQ_EVENT_NAME, msg.buffer);

	// TODO destroy things
	Timer_free(bomb->timer);
}
