#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include <stdbool.h>

typedef enum rotation {
	FIRST,
	SECOND,
	THIRD,
	FOURTH
} rotation;

typedef enum direction {
	LEFT = -1,
	NEUTRAL = 0,
	RIGHT = 1
} direction;

typedef struct half {
	entity* h_entity;
	int row;
	int column;
} half;

typedef struct pill {
	half left;
	half right;
	rotation r_state;
	bool active;
} pill;

pill init_pill(entity* entities_p);

bool get_pill_active(pill* pl);

bool set_pill_active(pill* pl, bool value);

rotation get_rotation_state(pill* pl);

void set_rotation_state(pill* pl, rotation r_state);

int get_left_row(pill* pl);

int get_left_column(pill* pl);

int get_right_row(pill* pl);

int get_right_column(pill* pl);

entity_type get_left_type(pill* pl);

#endif // PLAYER_H