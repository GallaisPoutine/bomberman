//
// Created by raphael on 03/04/20.
//

#include "Graphics.h"

#include <assert.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
// sleep
#include <unistd.h>

#define GROUND_PAIR (1)
#define WATER_PAIR  (2)
#define WALL_PAIR   (3)

#define BOMB        "0"
#define EMPTY       " "
#define EXPLOSION   "~"
#define PLAYER      "X"

#define RESET "R"

// SINGLETON ???
static WINDOW * window;

static WINDOW *Window_init(void) {
    WINDOW *this = initscr();
    int error = raw();
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to init graphics.\n");
        exit(EXIT_FAILURE);
    }

    error = noecho();
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to init graphics.\n");
        exit(EXIT_FAILURE);
    }

    error = keypad(stdscr, TRUE);
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to init graphics.\n");
        exit(EXIT_FAILURE);
    }
    
    error = start_color();
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to init graphics.\n");
        exit(EXIT_FAILURE);
    }

    // TODO awful tastes
    // change that !!!
    error = init_pair(GROUND_PAIR, COLOR_YELLOW, COLOR_GREEN);
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to init graphics.\n");
        exit(EXIT_FAILURE);
    }

    error = init_pair(WATER_PAIR, COLOR_CYAN, COLOR_BLUE);
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to init graphics.\n");
        exit(EXIT_FAILURE);
    }

    error = init_pair(WALL_PAIR, COLOR_RED, COLOR_MAGENTA);
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to init graphics.\n");
        exit(EXIT_FAILURE);
    }

    return this;
}

extern void Window_exit(void) { //WINDOW *this) {
    int error = endwin();
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to exit.\n");
        exit(EXIT_FAILURE);
    }
}

static void Window_refresh(void) {
    int error = refresh();
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to refresh graphics.\n");
        exit(EXIT_FAILURE);
    }
}

static void Window_clear(void) {
    int error = clear();
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to init graphics.\n");
        exit(EXIT_FAILURE);
    }
}

static void Window_display_center(char *msg, ...) {
    Window_clear();
    Window_display(LINES/2, (COLS/2) -(strlen(msg)/2), msg);
}

extern void Window_display(int x, int y, char *msg, ...) {
    int error = mvprintw(x, y, msg);
    if (error == ERR) {
        fprintf(stderr, "Fatal: unable to display graphics.\n");
        exit(EXIT_FAILURE);
    }

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
                attron(COLOR_PAIR(GROUND_PAIR));
                if (Tile_has_player(t)) {
                    Window_display(i, j, PLAYER);
                } else if(Tile_has_bomb(t)) {
                    Window_display(i, j, BOMB);
                } else {
                    // ASSERT ?
                    Window_display(i, j, EMPTY);
                }
                attroff(COLOR_PAIR(GROUND_PAIR));
                break;
            case WATER:
                attron(COLOR_PAIR(WATER_PAIR));
                if (Tile_has_player(t)) {
                    Window_display(i, j, PLAYER);
                } else if(Tile_has_bomb(t)) {
                    Window_display(i, j, BOMB);
                } else {
                    // ASSERT ?
                    Window_display(i, j, EMPTY);
                }
                attroff(COLOR_PAIR(WATER_PAIR));
                break;
            case WALL:
                attron(COLOR_PAIR(WALL_PAIR));
                if (Tile_has_player(t)) {
                    Window_display(i, j, PLAYER);
                } else if(Tile_has_bomb(t)) {
                    Window_display(i, j, BOMB);
                } else {
                    // ASSERT ?
                    Window_display(i, j, EMPTY);
                }
                attroff(COLOR_PAIR(WALL_PAIR));
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