//
// Created by raphael on 17/02/20.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H

#include "Bomb.h"

/**
 * Encapsulated Player structure
 */
typedef struct Player_t Player;

typedef enum Player_move_t{
    NONE = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
} Move;

/**
 * Create a new player and allocate memory
 * Init all parameters to zero
 *
 * @return player created
 */
extern Player *Player_new(int x, int y);

/**
 * Free the player from memory
 *
 * @param this
 */
extern void Player_free(Player *this);

/**
 * Return the x coordinate of the Player
 *
 * @param this
 * @return x
 */
extern int Player_get_X(Player *this);

/**
 * Set the x coordinate of the Player
 *
 * @param this
 * @param x
 */
extern void Player_set_X(Player * this, int x);

/**
 * Return the y coordinate of the Player
 *
 * @param this
 * @return
 */
extern int Player_get_Y(Player *this);

/**
 * Set the y coordinate of the Player
 *
 * @param this
 * @param y
 */
extern void Player_set_Y(Player *this, int y);

extern void Player_move(Player *this, Move m);

/**
 *
 * @param this
 * @return
 */
extern Bomb *Player_pose_bomb(Player *this);

#endif //BOMBERMAN_PLAYER_H
