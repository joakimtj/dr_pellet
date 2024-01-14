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

entity* get_left_entity(pill* pl);

entity* get_right_entity(pill* pl);

void set_left_entity(pill* pl, entity* entities_p, int pill_count);

void set_right_entity(pill* pl, entity* entities_p, int pill_count);

rotation get_rotation_state(pill* pl);

void set_rotation_state(pill* pl, rotation r_state);

int get_left_row(pill* pl);

int set_left_row(pill* pl, int row);

int get_left_column(pill* pl);

int get_right_row(pill* pl);

int set_right_row(pill* pl, int row);

int get_right_column(pill* pl);

entity_type get_left_type(pill* pl);

/*
* The below two functions calculates the difference in rows between
* the left and right segments of the 'pill'.
* This is used when determining the starting position of a new pill
* as a new pill retains it's rotation r_state.
* 
* @param pl A pointer to a pill struct. Must contain both left and right segments.
* @return The difference between the left half's row and right half's row.
*/

int calculate_difference_left(pill* pl);

int calculate_difference_right(pill* pl);

#endif // PLAYER_H