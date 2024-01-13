#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include "config.h"
#include "util.h"
#include "player.h"

typedef struct cell {
	entity* c_entity;
	bool is_player;
} cell;

void init_grid(cell grid[GRID_ROWS][GRID_COLUMNS], entity* entities);

void clear_virus_vertical(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column);

entity* get_grid_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j);

void set_initial_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

SDL_Rect* get_rect(cell c);

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r);

void set_grid_position_rect(SDL_Rect* rect, int i, int j);

int check_grid_index_bounds(int i);

int check_cell_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j);

int check_cell_player(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j);

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r);

void set_player_position_rect(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

void move_player_grid(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r);

int rotate_player_grid(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, rotation rot, bool reverse);

int check_left_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l);

int check_right_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_r, int column_axis_r);

#endif // GRID_H