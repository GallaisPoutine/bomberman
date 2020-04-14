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
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Field.h"
#include "Graphics.h"
#include "Player.h"
#include "Position.h"

#define ever (;;)

#define MQ_NAME  "/mq_event"
#define MQ_MAX_MESSAGES (10)
#define MAX_MSG_SIZE (1024)

#define END_OF_STR '\0'

typedef union {
    char msg;
    char buffer[MAX_MSG_SIZE];
} adapter_t;

static mqd_t mqueue;
static pthread_t thread_keypad_listener;

static void minit(void) {
    // Paramètres de la BAL
    struct mq_attr attr = {
        .mq_maxmsg = MQ_MAX_MESSAGES,
        .mq_msgsize = MAX_MSG_SIZE,
        .mq_flags = 0,
        .mq_curmsgs = 0
    };

    /* Création et ouverture de la BAL */
    mqueue = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0777, &attr);
    if (mqueue == -1) {
        GameEngine_stop();
        perror("ERROR creating mqueue");
        exit(EXIT_FAILURE);
    }

    int error = mq_close(mqueue);
    if (error == -1) {
        GameEngine_stop();
        perror("ERROR closing mqueue");
        exit(EXIT_FAILURE);
    }
}

static void msend(char *buffer) {
    mqueue = mq_open(MQ_NAME, O_WRONLY);
    if (mqueue == -1) {
        perror("ERROR opening mqueue");
        exit(EXIT_FAILURE);
    }

    /* Envoi de l'ordre à travers la BAL */
    int error = mq_send(mqueue, buffer, MAX_MSG_SIZE, 0);
    if (error == -1) {
        perror("ERROR sending char to mqueue");
        exit(EXIT_FAILURE);
    }

    error = mq_close(mqueue);
    if (error == -1) {
        GameEngine_stop();
        perror("ERROR closing mqueue");
        exit(EXIT_FAILURE);
    }
}

static void mreceive(char *buffer) {
    mqueue = mq_open(MQ_NAME, O_RDWR);
    if (mqueue == -1) {
        perror("ERROR opening mqueue");
        exit(EXIT_FAILURE);
    }

    int error = (int)mq_receive(mqueue, buffer, sizeof(adapter_t), 0);
    if (error == -1) {
        perror("ERROR receiving message in mqueue");
        exit(EXIT_FAILURE);
    }

    error = mq_close(mqueue);
    if (error == -1) {
        GameEngine_stop();
        fprintf(stderr, "[mreceive]\n");
        perror("ERROR closing mqueue");
        exit(EXIT_FAILURE);
    }
}

static void *keypad_listener() {
    adapter_t msg;
    for ever {
        msg.msg = Graphics_getch();
        if (msg.msg == 'z' || msg.msg == 'q' ||
            msg.msg == 's' || msg.msg == 'd' ||
            msg.msg == 'p') {
            msend(msg.buffer);
        }
    }
    return NULL;
}

static void run(void) {
    adapter_t msg = {.msg = 0};
    
    for ever {
        mreceive(msg.buffer);

        if (msg.msg == 'o') {
            GameEngine_stop();
            exit(EXIT_SUCCESS);
        } else {
            Window_display(20, 0, "%c", msg.msg);
        }
    }
}

extern void GameEngine_start(void) {
    minit();
    Graphics_greetings();
    
    int error = pthread_create(&thread_keypad_listener, NULL,
                                &keypad_listener, NULL);
    if (error != 0) {
        GameEngine_stop();
        perror("Fatal: ERROR creating keypad thread.\n");
        exit(EXIT_FAILURE);
    }

    Field *f = Field_new(10, 50);
    Player *p = Player_new(2, 25);
    Field_add_player(f, p);
    Graphics_display_field(f);
    Player_pose_bomb(p);
    
    run();


    Player_free(p);
    Field_free(f);

    sleep(2);
    GameEngine_stop();
}

extern void GameEngine_stop(void) {
    Window_exit();

    int error = mq_unlink(MQ_NAME);
    if (error == -1) {
        perror("ERROR unlinking mqueue");
        exit(EXIT_FAILURE);
    }

    error = pthread_cancel(thread_keypad_listener);
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