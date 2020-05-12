//
// Created by raphael on 03/04/20.
//

#ifndef BOMBERMAN_GRAPHICS_H
#define BOMBERMAN_GRAPHICS_H

#include "Field.h"

extern int Graphics_getch(void);

extern void Graphics_greetings(void);

extern void Window_exit(void);

extern void Window_display(int x, int y, char *msg, ...);

extern void Graphics_display_field(Field *field);

extern void Graphics_bomb_animation(Field *field, Bomb *bomb);

#endif /* BOMBERMAN_GRAPHICS_H */
