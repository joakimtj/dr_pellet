#include "player.h"

pill init_pill(entity* entities_p)
{
	half left = { &entities_p[0], 0, GRID_COLUMNS / 2 - 1 };
	half right = { &entities_p[1], 0, GRID_COLUMNS / 2 };

	return (pill) { left, right, FIRST, true };
}

int get_left_row(pill* pl)
{
	return pl->left.row;
}

int get_left_column(pill* pl)
{
	return pl->left.column;
}

int get_right_row(pill* pl)
{
	return pl->right.row;
}

int get_right_column(pill* pl)
{
	return pl->right.column;
}

entity_type get_left_type(pill* pl)
{
	return pl->left.h_entity->type;
}
