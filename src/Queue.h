//
// Created by raphael on 18/04/20.
//

#ifndef BOMBERMAN_QUEUE_H
#define BOMBERMAN_QUEUE_H

#define EV_UP       'z'
#define EV_DOWN     's'
#define EV_LEFT     'q'
#define EV_RIGHT    'd'
#define EV_PAUSE    'p'
#define EV_BOMB     'j'
#define EV_EXPLODE  'e'

#define MQ_NAME  "/mq_event"
#define MQ_MAX_MESSAGES (10)
#define MAX_MSG_SIZE (1024)

typedef union {
    char msg;
    char buffer[MAX_MSG_SIZE];
} adapter_t;

extern void Queue_init(void);

extern void Queue_send(char *buffer);

extern void Queue_receive(char *buffer);

extern void Queue_unlink(void);

#endif //BOMBERMAN_QUEUE_H
