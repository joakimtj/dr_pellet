#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "grid.h"
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

void enable_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

void disable_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

void remove_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

void set_player_position_rect(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

void move_player(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r);

int rotate_player(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, rotation* rot, bool reverse);

int check_left_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l);

int check_right_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_r, int column_axis_r);

#endif