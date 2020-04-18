//
// Created by raphael on 17/02/20.
//

/**
 * IDEA BOX
 *
 * Use mqueue to manage events.
 *
 * Compose with multiple windows
 * to allow log messages.
 *
 * Put all free in GE_stop
 * (Multiple arguments function ?).
 *
 *
 **/

#include "GameEngine.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
        if (msg.msg == 'z' || msg.msg == 'q' ||
            msg.msg == 's' || msg.msg == 'd' ||
            msg.msg == 'p') {
            Queue_send(msg.buffer);
        }
    }
    return NULL;
}

static void run(Field *f, Player *p) {
    adapter_t msg = {.msg = 0};
    for ever {
        Queue_receive(msg.buffer);

        switch (msg.msg) {
            case 'z':
                Field_move_player(f, p, UP);
                break;
            case 'q':
                Field_move_player(f, p, LEFT);
                break;
            case 's':
                Field_move_player(f, p, DOWN);
                break;
            case 'd':
                Field_move_player(f, p, RIGHT);
                break;
            case 'p':
                GameEngine_stop();
                exit(EXIT_SUCCESS);
            case 'e':
                // TODO manage
                Field_move_player(f, p, RIGHT);
                break;
            default:
                break;
        }

        Graphics_display_field(f);
    }
}

extern void GameEngine_start(void) {
    Queue_init();
    Graphics_greetings();

    int error = pthread_create(&thread_keypad_listener, NULL,
                                &keypad_listener, NULL);
    if (error != 0) {
        GameEngine_stop();
        perror("Fatal: ERROR creating keypad thread.\n");
        exit(EXIT_FAILURE);
    }

    Field *f = Field_new(10, 50);
    Player *p = Player_new(2, 24);
    Field_add_player(f, p);
    Graphics_display_field(f);
    Player_pose_bomb(p);

    run(f, p);

    Player_free(p);
    Field_free(f);

    sleep(2);
    GameEngine_stop();
}

extern void GameEngine_stop(void) {
    Window_exit();
    Queue_unlink();

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
