//
// Created by raphael on 03/04/20.
//

#ifndef BOMBERMAN_GRAPHICS_H
#define BOMBERMAN_GRAPHICS_H

#include "Field.h"

extern void Graphics_greetings(void);

extern void Window_exit(void);

extern void Window_display(int x, int y, char *msg, ...);

extern void Graphics_display_field(Field *field);

#endif /* BOMBERMAN_GRAPHICS_H */
