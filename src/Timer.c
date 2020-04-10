//
// Created by raphael on 17/02/20.
//

#include "Timer.h"

#include <assert.h>
#include <errno.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define TIME_UNIT (1) // Milliseconds

struct Timer_t{
    timer_t timer; // POSIX Timer
    uint32_t delay; /**< configured delay */
    Timer_callback callback; /**< function to be called at delay expiration */
    void * bomb; // arg to put in callback function ?
};

static void mainHandler (union sigval handlerParam){
    Timer *theTimer = handlerParam.sival_ptr; // Getting the timer reference
    theTimer->callback(theTimer->bomb); // Calling the callback function
}

extern Timer * Timer_new (uint32_t thisDelay, Timer_callback callback, void * bomb){
    Timer * newInstance;
    int error;

    // allocates and initializes the watchdog's attributes
    newInstance = (Timer *) malloc(sizeof(Timer));
    if (newInstance == NULL) {
        fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n", sizeof(Timer));
        exit(EXIT_FAILURE);
    }

    newInstance->delay = thisDelay;
    newInstance->callback = callback;
    newInstance->bomb = bomb;

    // Create the POSIX timer
    struct sigevent event = {
            .sigev_notify = SIGEV_THREAD,
            .sigev_notify_function = mainHandler,
            .sigev_notify_attributes = NULL,
            .sigev_value.sival_ptr = (void *) newInstance
    };

    error = timer_create(CLOCK_REALTIME, &event, &newInstance->timer);
    if (error == -1) {
        fprintf(stderr, "Fatal: unable to create timer - %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return newInstance;
}

extern void Timer_free (Timer *this){
    // Disarms and deletes the POSIX timer
    int error;

    Timer_cancel(this);
    error = timer_delete(this->timer);
    if (error == -1) {
        fprintf(stderr, "Fatal: unable to delete timer - %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Then we can free memory
    assert(this != NULL);
    free (this);
    this = NULL;
}

extern void Timer_start (Timer * this){
    //Starts the POSIX timer
    int error;
    struct itimerspec time;

    time.it_value.tv_sec = this->delay / TIME_UNIT; // Seconds
    time.it_value.tv_nsec = (this->delay % TIME_UNIT) * (1000000000 / TIME_UNIT); // Nanoseconds
    time.it_interval.tv_sec = time.it_value.tv_sec;
    time.it_interval.tv_nsec = time.it_value.tv_nsec;

    // Starts the POSIX timer
    error = timer_settime(this->timer, 0, &time, NULL);
    if (error == -1) {
        fprintf(stderr, "Fatal: unable to start timer - %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

extern void Timer_cancel(Timer *this){
    int error;
    struct itimerspec time;

    time.it_value.tv_sec = 0;
    time.it_value.tv_nsec = 0;
    time.it_interval.tv_sec = time.it_value.tv_sec;
    time.it_interval.tv_nsec = time.it_value.tv_nsec;

    // Disarms the POSIX timer
    error = timer_settime(this->timer, 0, &time, NULL);
    if (error == -1) {
        fprintf(stderr, "Fatal: unable to cancel timer - %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

extern uint32_t Timer_get_delay(Timer * this){
    return this->delay;
}

extern void Timer_set_delay(Timer * this, uint32_t delay){
    this->delay = delay;
}
