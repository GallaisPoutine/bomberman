//
// Created by raphael on 17/02/20.
//

#ifndef BOMBERMAN_TIMER_H
#define BOMBERMAN_TIMER_H

#include <signal.h>
#include <stdint.h>
#include <time.h>

#define NOMINAL_BOMB_DELAY (1) // 1s

typedef void (*Timer_callback)(void * field);

struct Timer {
	timer_t timer; // POSIX Timer
	uint32_t delay; /**< configured delay */
	Timer_callback callback; /**< function to be called at delay expiration */
	void * bomb; // arg to put in callback function ?
};

extern struct Timer *Timer_new(uint32_t delay, Timer_callback callback, void * bomb);
extern void Timer_free(struct Timer *this);

extern void Timer_start(struct Timer * this);
extern void Timer_cancel(struct Timer *this);

#endif //BOMBERMAN_TIMER_H
