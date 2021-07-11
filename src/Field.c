//
// Created by raphael on 21/02/20.
//

#include "Field.h"

#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

#include "Queue.h"

#define Field_get_tile(field, x, y) field->grid[x][y]

extern struct Field *Field_new(int length, int depth) {
	struct Field *field = (struct Field *) malloc(sizeof(struct Field));
	if (!field) {
		fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n",
		sizeof(struct Field));
		exit(EXIT_FAILURE);
	}

	field->length = length;
	field->depth = depth;

	// Allocation de la matrice de pointeurs
	field->grid = (struct Tile ***)calloc(length, sizeof(struct Tile **));
	if (!field->grid) {
		fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n",
		sizeof(struct Tile **));
		exit(EXIT_FAILURE);
	}

	// Initialisation de chaque Tile dans la matrice
	for (int i=0; i<length; i++) {
		field->grid[i] = (struct Tile **) calloc(depth, sizeof(struct Tile *));
		if (!field->grid[i]) {
			fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n",
			sizeof(struct Tile *));
			exit(EXIT_FAILURE);
		}
		for (int j=0; j<depth; j++) {
			if (i == 0 || j == 0 ||
				i == length -1 || j == depth -1 ||
				(i%2 == 1 && j%2 == 1)) {
				field->grid[i][j] = Tile_new(WALL, i, j);
			} else {
				field->grid[i][j] = Tile_new(GROUND, i, j);
			}
		}
	}

	return field;
}

// FIXME memory leaks around here
extern void Field_free(struct Field *field) {
	assert(field != NULL);
	for (int i=0; i<field->length; i++) {
		assert(field->grid[i] != NULL);
		free(field->grid[i]);
		// for (int j=0; i<field->depth; j++) {
		//	 assert(field->grid[i][j] != NULL);
		//	 free(field->grid[i][j]);
		// }
	}
	free(field);
}

extern void Field_add_player(struct Field *field, struct Player *player){
	// TODO check if he's in a wall
	struct Tile * tile = Field_get_tile(field, player->pos->x, player->pos->y);

	assert(tile->type == GROUND);
	if (tile->type == GROUND) {
		Tile_add_player(tile, player);
	}
	printf("Player Added\n");
}

// TODO optimize switch case
extern void Field_move_player(struct Field *field, struct Player *player,
			      enum player_move m) {
	struct Tile *tile = Field_get_tile(field, player->pos->x,
					   player->pos->y);
	struct Tile *tile_next = Field_get_tile(field, player->pos->x,
						player->pos->y);

	switch (m) {
	case UP:
		player->pos->y--;
		break;
	case DOWN:
		player->pos->y++;
		break;
	case LEFT:
		player->pos->x--;
		break;
	case RIGHT:
		player->pos->x++;
		break;
	default:
		assert (m == NONE);
		break;
	}

	if (tile_next->type == GROUND) {
		Tile_remove_player(tile);
		Tile_add_player(tile_next, player);
		Player_move(player, m);
	}
}

extern void Field_bomb_has_been_planted(struct Field *field,
					struct Player *player) {
	bomb_adapter_t b = {.bomb = Player_pose_bomb(player)};
	struct Tile *tile = Field_get_tile(field, (b.bomb)->pos->x,
					   (b.bomb)->pos->y);
	assert(tile->type == GROUND);
	Tile_add_bomb(tile, b.bomb);
	Queue_send(MQ_BOMBS_NAME, b.buffer);
}

extern void Field_bomb_explosion(struct Field *field) {
	bomb_adapter_t b = {.buffer = 0};
	Queue_receive(MQ_BOMBS_NAME, b.buffer);
	struct Bomb *bomb = b.bomb;
	int xMin, xMax, yMin, yMax;

	if (bomb->pos->x - BOMB_INTENSITY >= 0)
		xMin = bomb->pos->x - BOMB_INTENSITY;
	else
		xMin = 0;

	if (bomb->pos->x + BOMB_INTENSITY < field->length)
		xMax = bomb->pos->x + BOMB_INTENSITY;
	else
		xMax = field->length - 1;

	for (int i = xMin; i <= xMax; i++) {
		struct Tile *ttmp = Field_get_tile(field, i, bomb->pos->y);
	if (ttmp->type == WALL) {
		break;
	}
		if (ttmp->type != WALL || ttmp->has_player) {
			Tile_remove_player(ttmp);
			Tile_destroy(ttmp);
		}
	}

	if (bomb->pos->y - BOMB_INTENSITY >= 0)
		yMin = bomb->pos->y - BOMB_INTENSITY;
	else
		yMin = 0;

	if (bomb->pos->y + BOMB_INTENSITY < field->depth)
		yMax = bomb->pos->y + BOMB_INTENSITY;
	else
		yMax = field->depth - 1;

	for (int j = yMin; j <= yMax; j++) {
		struct Tile *ttmp = Field_get_tile(field, bomb->pos->x, j);

	if (ttmp->type == WALL) {
		break;
	}
		if (ttmp->type != WALL || ttmp->has_player) {
			Tile_remove_player(ttmp);
			Tile_destroy(ttmp);
		}
	}

	Tile_remove_bomb(Field_get_tile(field, bomb->pos->x, bomb->pos->y));
}

extern void Field_fill(struct Field *field) {
	for (int i = 0; i < field->length; i++) {
		for (int j = 0; j < field->depth; j++) {
			struct Tile *ttmp = Field_get_tile(field, i, j);
			if (ttmp->type == GROUND && !ttmp->has_player) {
				Tile_fill(ttmp);
			}
		}
	}
}
