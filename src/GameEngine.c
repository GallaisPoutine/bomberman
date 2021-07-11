//
// Created by raphael on 17/02/20.
//

/**
 * IDEA BOX
 *
 * Compose with multiple windows
 * to allow log messages.
 *
 **/

#include "GameEngine.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "Field.h"
#include "Graphics.h"
#include "Player.h"
#include "Position.h"
#include "Queue.h"

#define ever (;;)

static pthread_t thread_keypad_listener;

static void *keypad_listener() {
    adapter_t msg;
    for ever {
        msg.msg = Graphics_getch();
        if (msg.msg == EV_UP || msg.msg == EV_DOWN ||
            msg.msg == EV_LEFT || msg.msg == EV_RIGHT ||
            msg.msg == EV_PAUSE || msg.msg == EV_BOMB) {
            Queue_send(MQ_EVENT_NAME, msg.buffer);
        }
    }
    return NULL;
}

static void free_entities(struct Field *f, struct Player *p) {
    Player_free(p);
    Field_free(f);
}

static void run(struct Field *f, struct Player *p) {
    adapter_t msg = {.msg = 0};
    for ever {
        Queue_receive(MQ_EVENT_NAME, msg.buffer);

        switch (msg.msg) {
        case EV_UP:
            Field_move_player(f, p, UP);
            break;
        case EV_LEFT:
            Field_move_player(f, p, LEFT);
            break;
        case EV_DOWN:
            Field_move_player(f, p, DOWN);
            break;
        case EV_RIGHT:
            Field_move_player(f, p, RIGHT);
            break;
        case EV_BOMB:
            Field_bomb_has_been_planted(f, p);
            break;
        case EV_EXPLODE:
            Field_bomb_explosion(f);
            break;
        case EV_PAUSE:
            GameEngine_stop();
            exit(EXIT_SUCCESS);
        default:
            break;
        }
        Graphics_display_field(f);
    }
}

#define RANDOM(__MIN__, __MAX__) (rand() % ((__MAX__) + 1 - (__MIN__)) + (__MIN__))

static void place_player(struct Field *f, struct Player *p, int xmax, int ymax) {
	// Formula to generate a random number within interval
	// rand() % (max_number + 1 - minimum_number) + minimum_number
	// Position *pplayer;
	int xrand, yrand;
	struct Tile *ttmp;

	do {
		xrand = RANDOM(0, xmax);
		yrand = RANDOM(0, ymax);
		ttmp = Field_get_tile(f, xrand, yrand);
	} while(ttmp->type != GROUND);

	p->pos->x = ++xrand;
	p->pos->y = ++yrand;
}

#define FIELD_WIDTH	70
#define FIELD_HEIGHT	30
extern void GameEngine_start(void) {
    struct Player *p;

    Queue_init(MQ_EVENT_NAME);
    Queue_init(MQ_BOMBS_NAME);
    Graphics_greetings();

    int error = pthread_create(&thread_keypad_listener, NULL,
                                &keypad_listener, NULL);
    if (error != 0) {
        GameEngine_stop();
        perror("Fatal: ERROR creating keypad thread.\n");
        exit(EXIT_FAILURE);
    }

    struct Field *f = Field_new(FIELD_WIDTH, FIELD_HEIGHT);
    p = Player_new(0, 0);
    place_player(f, p, FIELD_WIDTH, FIELD_HEIGHT);
    Field_add_player(f, p);
    Field_fill(f);
    Graphics_display_field(f);
    run(f, p);
    free_entities(f, p);

    GameEngine_stop();
}

extern void GameEngine_stop(void) {
    Window_exit();
    Queue_unlink(MQ_EVENT_NAME);
    Queue_unlink(MQ_BOMBS_NAME);

    int error = pthread_cancel(thread_keypad_listener);
    if (error != 0) {
        perror("Fatal: ERROR canceling keypad thread.\n");
        error = pthread_kill(thread_keypad_listener, SIGKILL);
        if (error != 0) {
            perror("Fatal: ERROR canceling keypad thread.\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("Exiting...\n");
}
