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

#include <stdio.h>
#include <unistd.h>

#include "Field.h"
#include "Graphics.h"
#include "Player.h"
#include "Position.h"

extern void GameEngine_start(void) {

    Graphics_greetings();

    Field *f = Field_new(4, 50);
    // Graphics_display_field(f);

    Player *p = Player_new(14, 25);
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
    printf("Exiting...\n");
}