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

void enable_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl)
{
	grid[pl->left.row][pl->left.column].is_player = true;
	grid[pl->right.row][pl->right.column].is_player = true;
}

void disable_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl)
{
	grid[pl->left.row][pl->left.column].is_player = false;
	grid[pl->right.row][pl->right.column].is_player = false;
}

void remove_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl)
{
	grid[pl->left.row][pl->left.column].c_entity = NULL;
	grid[pl->right.row][pl->right.column].c_entity = NULL;
}

void set_player_position_rect(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl)
{
	set_grid_position_rect(get_rect(grid[pl->left.row][pl->left.column]), pl->left.row, pl->left.column);
	set_grid_position_rect(get_rect(grid[pl->right.row][pl->right.column]), pl->right.row, pl->right.column);
}

void move_player(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r)
{
	// These temporary pointers allows the player blocks to move without respect to any order of operations.
	entity* tmp_1 = pl->left.h_entity;
	entity* tmp_2 = pl->right.h_entity;

	remove_pill(grid, pl);
	disable_pill(grid, pl);

	pl->left.row = pl->left.row + row_axis_l;
	pl->left.column = pl->left.column + column_axis_l;
	pl->right.row = pl->right.row + row_axis_r;
	pl->right.column = pl->right.column + column_axis_r;

	enable_pill(grid, pl);

	grid[pl->left.row][pl->left.column].c_entity = tmp_1;
	grid[pl->right.row][pl->right.column].c_entity = tmp_2;

	pl->left.h_entity = tmp_1;
	pl->right.h_entity = tmp_2;
}

int rotate_player(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, rotation* rot, bool reverse)
{
	int row_axis_l = 0, column_axis_l = 0;
	int row_axis_r = 0, column_axis_r = 0;
	switch (*rot) {
	case FIRST:
		row_axis_l = -1;
		column_axis_r = -1;
		break;

	case SECOND:
		row_axis_l = 1;
		column_axis_l = 1;
		break;

	case THIRD:
		column_axis_l = -1;
		row_axis_r = -1;
		break;

	case FOURTH:
		row_axis_r = 1;
		column_axis_r = 1;
		break;
	}

	if (reverse) {
		row_axis_l = -row_axis_l;
		column_axis_l = -column_axis_l;
		row_axis_r = -row_axis_r;
		column_axis_r = -column_axis_r;
	}
	// Wall kick off ceiling
	if (pl->left.row + row_axis_l < 0 || pl->right.row + row_axis_r < 0) {
		row_axis_l++;
		row_axis_r++;
	}
	// Wall kick off right wall
	if (pl->left.column + column_axis_l >= GRID_COLUMNS - 1 && pl->right.column + column_axis_r >= GRID_COLUMNS - 1) {
		column_axis_l--;
		column_axis_r--;
	}
	// Rotation collision
	if (check_cell_collision(grid, pl, row_axis_l, column_axis_l, row_axis_r, column_axis_r)) return -1;
	else move_player(grid, pl, row_axis_l, column_axis_l, row_axis_r, column_axis_r);

	return 0;
}

int check_left_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l)
{
	if ((grid[pl->left.row + row_axis_l][pl->left.column + column_axis_l].c_entity) && !(grid[pl->left.row + row_axis_l][pl->left.column + column_axis_l].is_player))
		return 1;
	else return 0;
}

int check_right_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_r, int column_axis_r)
{
	if ((grid[pl->right.row + row_axis_r][pl->right.column + column_axis_r].c_entity) && !(grid[pl->right.row + row_axis_r][pl->right.column + column_axis_r].is_player))
		return 1;
	else return 0;
}