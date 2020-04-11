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

#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Field.h"
#include "Graphics.h"
#include "Player.h"
#include "Position.h"

#define MQ_NAME  "/mq_event"
#define MQ_MAX_MESSAGES (10)
#define MAX_MSG_SIZE (1024)

static mqd_t mqueue;

static void keypad_listener(void) {
	/********* Système permettant la capture de touches sans entrée ********/
	char capture;
	/* use system call to make terminal send all keystrokes directly to stdin */
	system ("/bin/stty raw");
	capture = getchar();
	/* use system call to set terminal behaviour to more normal behaviour */
	system ("/bin/stty cooked");
	/***********************************************************************/
    if (capture == 'z' || capture == 'q' ||
        capture == 's' || capture == 'd' ||
        capture == 'p') {
            mqueue = mq_open(MQ_NAME, O_WRONLY);
            /* Envoi de l'ordre à travers la BAL */
            // &capture gonna be a problem
            int error = mq_send(mqueue, &capture, sizeof (char), 0);
            if (error != 0) {
                fprintf(stderr, "ERROR sending char to mqueue\n");
            }
            mq_close(mqueue);
	}
}

static void run(void) {
    char *msg = NULL;
    do {
        keypad_listener();

        mqueue = mq_open(MQ_NAME, O_RDWR);
        int error = (int)mq_receive (mqueue, msg, MAX_MSG_SIZE, 0);
        if (error == -1) {
            fprintf(stderr, "ERROR receiving message in mqueue\n");
            exit(EXIT_FAILURE);
        }
        mq_close(mqueue);

        printf("%s\n", msg);
    } while (1);
}

extern void GameEngine_start(void) {
    // Paramètres de la BAL
    struct mq_attr attr;
    attr.mq_maxmsg = MQ_MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;

    /* Unlinking causes bad behavior */
    mq_unlink(MQ_NAME);

    /* Création et ouverture de la BAL */
    mqueue = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0777, &attr);
    if (mqueue == -1) {
        fprintf(stderr, "ERROR creating mqueue\n");
        exit(EXIT_FAILURE);
    }

    Graphics_greetings();

    run();

    Field *f = Field_new(10, 50);
    // Graphics_display_field(f);

    Player *p = Player_new(2, 25);
    Field_add_player(f, p);
    Graphics_display_field(f);

    Bomb *b = Player_pose_bomb(p);

    Player_free(p);
    Field_free(f);

    sleep(2);
    GameEngine_stop();

    Window_exit();
}

extern void GameEngine_stop(void) {
    mqueue = mq_open(MQ_NAME, O_WRONLY);
    /* Envoi de l'ordre à travers la BAL */
    // &capture gonna be a problem
    int error = mq_send(mqueue, "EXIT", sizeof (char), 0);
    if (error != 0) {
        fprintf(stderr, "ERROR sending char to mqueue\n");
    }
    mq_close(mqueue);
    printf("Exiting...\n");
}