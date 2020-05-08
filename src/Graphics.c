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

static void Window_display_tile(Tile *t, int x, int y, int color) {
    attron(color);
    if (Tile_has_player(t)) {
        Window_display(x, y, PLAYER);
    } else if(Tile_has_bomb(t)) {
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
    Window_refresh();
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
extern void Graphics_display_field(Field *field) {
    Window_clear();
    int length = Field_get_length(field);
    int depth = Field_get_depth(field);
    for (int i=0; i<length; i++) {
        for (int j=0; j<depth; j++) {
            Tile *t = Field_get_tile(field, i, j);
            switch (Tile_get_type(t)) {
            case GROUND:
                Window_display_tile(t, j, i, COLOR_PAIR(GROUND_PAIR));
                break;
            case WATER:
                Window_display_tile(t, j, i, COLOR_PAIR(WATER_PAIR));
                break;
            case WALL:
                Window_display_tile(t, j, i, COLOR_PAIR(WALL_PAIR));
                break;
            default:
                assert(Tile_get_type(t) != GROUND);
                assert(Tile_get_type(t) != WATER);
                assert(Tile_get_type(t) != WALL);
                break;
            }
        }
    }
    Window_refresh();
}