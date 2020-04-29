//
// Created by raphael on 18/04/20.
//

#include "Queue.h"
#include "GameEngine.h"

#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>


extern void Queue_init(void) {
    // Paramètres de la BAL
    struct mq_attr attr = {
            .mq_maxmsg = MQ_MAX_MESSAGES,
            .mq_msgsize = MAX_MSG_SIZE,
            .mq_flags = 0,
            .mq_curmsgs = 0
    };

    /* Création et ouverture de la BAL */
    mqd_t mqueue = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0777, &attr);
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

extern void Queue_send(char *buffer) {
    mqd_t mqueue = mq_open(MQ_NAME, O_WRONLY);
    if (mqueue == -1) {
        perror("ERROR opening mqueue");
        exit(EXIT_FAILURE);
    }

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

extern void Queue_receive(char *buffer) {
    mqd_t mqueue = mq_open(MQ_NAME, O_RDWR);
    if (mqueue == -1) {
        perror("ERROR opening mqueue");
        exit(EXIT_FAILURE);
    }

    int error = (int)mq_receive(mqueue, buffer, MAX_MSG_SIZE, 0);
    if (error == -1) {
        perror("ERROR receiving message in mqueue");
        exit(EXIT_FAILURE);
    }

    error = mq_close(mqueue);
    if (error == -1) {
        GameEngine_stop();
        fprintf(stderr, "[Queue_receive]\n");
        perror("ERROR closing mqueue");
        exit(EXIT_FAILURE);
    }
}

extern void Queue_unlink(void) {
    int error = mq_unlink(MQ_NAME);
    if (error == -1) {
        perror("ERROR unlinking mqueue");
        exit(EXIT_FAILURE);
    }
}