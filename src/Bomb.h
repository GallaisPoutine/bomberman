//
// Created by raphael on 17/02/20.
//

#ifndef BOMBERMAN_BOMB_H
#define BOMBERMAN_BOMB_H

/**
 *
 */
typedef struct Bomb_t Bomb;

/**
 *
 * @return
 */
extern Bomb * Bomb_new(int x, int y);

/**
 *
 * @param this
 */
extern void Bomb_free(Bomb *this);

/**
 *
 * @param this
 * @return
 */
extern int Bomb_get_X(Bomb *this);

/**
 *
 * @param this
 * @param x
 */
extern void Bomb_set_X(Bomb *this, int x);

/**
 *
 * @param this
 * @return
 */
extern int Bomb_get_Y(Bomb *this);

/**
 *
 * @param this
 * @param y
 */
extern void Bomb_set_Y(Bomb *this, int y);

/**
 *
 * @param this
 */
extern void Bomb_start_timer(Bomb *this);

extern void Bomb_explode(Bomb *this);

#endif //BOMBERMAN_BOMB_H
