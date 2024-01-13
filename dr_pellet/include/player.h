#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "config.h"
#include <stdbool.h>

typedef enum rotation {
	FIRST,
	SECOND,
	THIRD,
	FOURTH
} rotation;

typedef struct half {
	entity* h_entity;
	int row;
	int column;
} half;

typedef struct pill {
	half left;
	half right;
	rotation state;
	bool active;
} pill;

pill init_pill(entity* entities_p);

int get_left_row(pill* pl);

int get_left_column(pill* pl);

int get_right_row(pill* pl);

int get_right_column(pill* pl);

entity_type get_left_type(pill* pl);

#endif