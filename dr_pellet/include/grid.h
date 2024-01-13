#ifndef GRID_H
#define GRID_H

#define GRID_ROWS 16 // The height of the play-area.
#define GRID_COLUMNS 8 // The width of the play-area.
#define GRID_ROW_CLEARANCE 8 // Viruses will not spawn above or on this ROW.
#define RECT_SIZE 30
#define PADDING 1

#include <stdio.h>
#include <stdbool.h>
#include "entity.h"
#include "util.h"
#include "render.h"
#include "player.h"

struct {
	entity* c_entity;
	bool is_player;
} typedef cell;

void init_grid(cell grid[GRID_ROWS][GRID_COLUMNS], entity* entities);

void clear_virus_vertical(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column);

entity* get_grid_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j);

void set_initial_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl);

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r);

void set_grid_position_rect(SDL_Rect* rect, int i, int j);

int check_cell_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j);

int check_cell_player(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j);

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r);

#endif