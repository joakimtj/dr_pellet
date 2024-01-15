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

entity* get_cell_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j);

void delete_cell_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int row, int column);

entity_type get_cell_type(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j);

int check_cell_type(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j, entity_type type);

void clear_virus_horizontal(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column);

int check_entities_below(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column);

int check_entities_above(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column);

int check_entities_right(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column);

int check_entities_left(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column);

void delete_entities_below(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column, int total_below);

void delete_entities_above(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column, int total_above);

void delete_entities_right(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column, int total_right);

void delete_entities_left(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column, int total_left);

void clear_viruses(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column);

void set_initial_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

SDL_Rect* get_rect(cell c);

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r);

void set_grid_position_rect(SDL_Rect* rect, int i, int j, int offset);

void interpolate_grid_position_rect(SDL_Rect* rect, int start_i, int end_i, int j, float interp_factor);

int check_grid_index_bounds(int i);

int check_cell_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j);

int check_cell_player(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j);

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r);

void enable_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

void disable_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

void remove_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

void set_player_position_rect(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

int check_left_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l);

int check_right_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_r, int column_axis_r);

void move_player_grid(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r);

void update_player_grid(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int gravity);

int rotate_player_grid(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, bool reverse);

#endif // GRID_H