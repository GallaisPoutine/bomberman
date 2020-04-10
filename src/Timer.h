//
// Created by raphael on 17/02/20.
//

#ifndef BOMBERMAN_TIMER_H
#define BOMBERMAN_TIMER_H

#include <stdint.h>

#define NOMINAL_BOMB_DELAY (1) // 1s

typedef struct Timer_t Timer;

typedef void (*Timer_callback)(void * field);

extern Timer * Timer_new(uint32_t delay, Timer_callback callback, void * bomb);
extern void Timer_free(Timer *this);

extern void Timer_start(Timer * this);
extern void Timer_cancel(Timer *this);

extern uint32_t Timer_get_delay(Timer * this);
extern void Timer_set_delay(Timer * this, uint32_t delay);

#endif //BOMBERMAN_TIMER_H
