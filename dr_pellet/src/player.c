#include "player.h"

pill init_pill(entity* entities_p)
{
	half left = { &entities_p[0], 0, GRID_COLUMNS / 2 - 1 };
	half right = { &entities_p[1], 0, GRID_COLUMNS / 2 };

	return (pill) { left, right, FIRST, true };
}

bool get_pill_active(pill* pl) 
{
	return pl->active;
}

bool set_pill_active(pill* pl, bool value)
{
	pl->active = value;
}

entity* get_left_entity(pill* pl)
{
	return pl->left.h_entity;
}

entity* get_right_entity(pill* pl)
{
	return pl->right.h_entity;
}

void set_left_entity(pill* pl, entity* entities_p, int pill_count)
{
	pl->left.h_entity = &entities_p[pill_count];
}

void set_right_entity(pill* pl, entity* entities_p, int pill_count)
{
	pl->right.h_entity = &entities_p[pill_count];
}

rotation get_rotation_state(pill* pl)
{
	return pl->r_state;
}

void set_rotation_state(pill* pl, rotation state)
{
	pl->r_state = state;
}

int get_left_row(pill* pl)
{
	return pl->left.row;
}

int get_left_column(pill* pl)
{
	return pl->left.column;
}

int set_left_row(pill* pl, int value)
{
	printf("Left: %d\n", value);
	pl->left.row = value;
}

int get_right_row(pill* pl)
{
	return pl->right.row;
}

int set_right_row(pill* pl, int value)
{
	printf("Right: %d\n", value);
	pl->right.row = value;
}

int get_right_column(pill* pl)
{
	return pl->right.column;
}

int calculate_difference_left(pill* pl)
{
	int difference_left = pl->left.row - pl->right.row;

	if (difference_left > 0) {
		return difference_left;
	}
	else {
		return 0;
	}
}

int calculate_difference_right(pill* pl)
{
	int difference_right = pl->right.row - pl->left.row;

	if (difference_right > 0) {
		return difference_right;
	}
	else {
		return 0;
	}
}

entity_type get_left_type(pill* pl)
{
	return pl->left.h_entity->type;
}