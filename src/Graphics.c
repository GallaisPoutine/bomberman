//
// Created by raphael on 03/04/20.
//

#include "Graphics.h"

#include <assert.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
// sleep
#include <unistd.h>

#define GROUND_PAIR (1)
#define WATER_PAIR  (2)
#define WALL_PAIR   (3)

#define BOMB        "0"
#define EMPTY       " "
#define EXPLOSION   "#"
#define PLAYER      "X"

#define RESET "R"

// SINGLETON ???
static WINDOW * window;

static void Window_throw_error(bool b, char* msg) {
    if (b) {
        fprintf(stderr, "%s\n", msg);
        exit(EXIT_FAILURE);
    }
}

static WINDOW *Window_init(void) {
    WINDOW *this = initscr();
    int error = raw();
    Window_throw_error(error == ERR, "Fatal: unable to init graphics.");

    error = noecho();
    Window_throw_error(error == ERR, "Fatal: unable to init graphics.");

    error = keypad(stdscr, TRUE);
    Window_throw_error(error == ERR, "Fatal: unable to init graphics.");

    error = start_color();
    Window_throw_error(error == ERR, "Fatal: unable to init graphics.");

    error = curs_set(0);
    Window_throw_error(error == ERR, "Fatal: unable to init graphics.");

    // TODO awful tastes
    // change that !!!
    error = init_pair(GROUND_PAIR, COLOR_YELLOW, COLOR_GREEN);
    Window_throw_error(error == ERR, "Fatal: unable to init graphics.");

    error = init_pair(WATER_PAIR, COLOR_CYAN, COLOR_BLUE);
    Window_throw_error(error == ERR, "Fatal: unable to init graphics.");

    error = init_pair(WALL_PAIR, COLOR_RED, COLOR_MAGENTA);
    Window_throw_error(error == ERR, "Fatal: unable to init graphics.");

    return this;
}

extern void Window_exit(void) {
    int error = endwin();
    Window_throw_error(error == ERR, "Fatal: unable to exit.");
}

static void Window_refresh(void) {
    int error = refresh();
    Window_throw_error(error == ERR, "Fatal: unable to refresh graphics.");
}

static void Window_clear(void) {
    int error = clear();
    Window_throw_error(error == ERR, "Fatal: unable to clear graphics.");
}

static void Window_display_center(char *msg, ...) {
    Window_clear();
    Window_display(LINES/2, (COLS/2) -(strlen(msg)/2), msg);
}

static void Window_display_tile(struct Tile *tile, int x, int y, int color) {
    attron(color);
    if (tile->has_player) {
        Window_display(x, y, PLAYER);
    } else if(tile->has_bomb) {
        Window_display(x, y, BOMB);
    } else {
        // ASSERT ?
        Window_display(x, y, EMPTY);
    }
    attroff(color);
}

extern void Window_display(int x, int y, char *msg, ...) {
    int error = mvprintw(x, y, msg);
    Window_throw_error(error == ERR, "Fatal: unable to display graphics.");
}

extern int Graphics_getch(void) {
    int ch = getch();
    if (ch == ERR) {
        fprintf(stderr, "Unable to get key pressed.\n");
        // exit(EXIT_FAILURE);
    }

    return ch;
}

extern void Graphics_greetings(void) {
    window = Window_init();
    Window_display_center("WELCOME TO BOMBERMAN v1");
    Window_refresh();
}

/**
 * Premier essai
 **/
extern void Graphics_display_field(struct Field *field) {
    Window_clear();

    for (int i = 0; i < field->length; i++) {
        for (int j = 0; j < field->depth; j++) {
            struct Tile *tile = Field_get_tile(field, i, j);
            switch (tile->type) {
            case GROUND:
                Window_display_tile(tile, j, i, COLOR_PAIR(GROUND_PAIR));
                break;
            case WATER:
                Window_display_tile(tile, j, i, COLOR_PAIR(WATER_PAIR));
                break;
            case WALL:
                Window_display_tile(tile, j, i, COLOR_PAIR(WALL_PAIR));
                break;
            default:
                assert(tile->type != GROUND);
                assert(tile->type != WATER);
                assert(tile->type != WALL);
                break;
            }
        }
    }
    Window_refresh();
}

extern void Graphics_bomb_animation(struct Field *this, struct Bomb *bomb) {
    int xMin, xMax, yMin, yMax;

    if (bomb->pos->x - BOMB_INTENSITY >= 0)
        xMin = bomb->pos->x - BOMB_INTENSITY;
    else
        xMin = 0;

    if (bomb->pos->x + BOMB_INTENSITY < this->length)
        xMax = bomb->pos->x + BOMB_INTENSITY;
    else
        xMax = this->length - 1;

    for (int i = xMin; i <= xMax; i++) {
        struct Tile *ttmp = Field_get_tile(this, i, bomb->pos->y);
        if (ttmp->type != WALL && ttmp->has_player) {
            Window_display(i, bomb->pos->y, "#");
        }
    }

    if (bomb->pos->y - BOMB_INTENSITY >= 0)
        yMin = bomb->pos->y - BOMB_INTENSITY;
    else
        yMin = 0;

    if (bomb->pos->y + BOMB_INTENSITY < this->depth)
        yMax = bomb->pos->y + BOMB_INTENSITY;
    else
        yMax = this->depth - 1;

    for (int j = yMin; j <= yMax; j++) {
        struct Tile *ttmp = Field_get_tile(this, bomb->pos->x, j);
        if (ttmp->type != WALL && ttmp->has_player) {
            Window_display(bomb->pos->x, j, "#");
        }
    }
}
