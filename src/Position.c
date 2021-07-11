//
// Created by raphael on 17/02/20.
//

#include "Position.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>


extern struct Position *Position_new(int x, int y) {
	struct Position *pos = (struct Position *) malloc(sizeof(struct Position));
	if (!pos) {
		fprintf(stderr, "Fatal: unable to allocate %zu bytes.\n",
			sizeof(struct Position));
		exit(EXIT_FAILURE);
	}

	pos->x = x;
	pos->y = y;

	return pos;
}

extern void Position_free(struct Position *pos) {
	assert(pos != NULL);

	if (!pos)
		return;

	free(pos);
}
