#include "grid.h"

void init_grid(cell grid[GRID_ROWS][GRID_COLUMNS], entity* entities)
{
	int entity_count = 0;
	int random_i;
	for (int i = 0; i < GRID_ROWS; i++)
	{
		for (int j = 0; j < GRID_COLUMNS; j++)
		{
			if (i > GRID_ROW_CLEARANCE)
			{
				if (entity_count < VIRUS_N)
				{
					random_i = get_random_integer(2);
					if (random_i == 0) {
						grid[i][j].c_entity = NULL;
					}
					else {
						grid[i][j].c_entity = &entities[entity_count];
						set_grid_position_rect(&grid[i][j].c_entity->rect, i, j);
						entity_count++;
					}
				}
				else {
					grid[i][j].c_entity = NULL;
				}
			}
			else {
				grid[i][j].c_entity = NULL;
			}
			grid[i][j].is_player = false;
		}
	}
	printf("in func, init_grid()\nentities set to grid: %d\n", entity_count);
}

void clear_virus_vertical(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column)
{
	int total_below = 0;
	int total_above = -1; // This is done so that entity on row, column, isn't counted twice.
	int total_sum = 0;

	int to_check = 1;
	for (int i = row; i < i + to_check; i++)
	{
		if (i < GRID_ROWS - 1)
		{
			if (grid[i][column].c_entity)
			{
				if (grid[row][column].c_entity->type == grid[i][column].c_entity->type)
				{
					to_check++;
					total_below++;
				}
				else break;
			}
			else break;
		}
		else break;
	}

	to_check = 1;
	for (int i = row; i > i - to_check; i--)
	{
		printf("In above\n");
		if (i > 0)
		{
			printf("Entity: %p\n", grid[i][column].c_entity);
			if (grid[i][column].c_entity)
			{
				if (grid[row][column].c_entity->type == grid[i][column].c_entity->type)
				{
					to_check++;
					total_above++;
				}
				else break;
			}
			else break;
		}
		else break;
	}

	total_sum = total_below + (total_above);
	printf("Total below: %d\n", total_below);
	printf("Total above: %d\n", total_above);
	printf("Sum of total: %d\n", total_sum);
	if (total_sum > 3)
	{
		for (int i = 0; i < total_below; i++)
		{
			if (i <= GRID_ROWS - 1)
			{
				if (check_cell_entity(grid, i + row, column))
					grid[i + row][column].c_entity = NULL;
				else break;
			}
			else break;
		}

		for (int i = 0; i >= total_above; i--)
		{
			if (i >= 0)
			{
				if (check_cell_entity(grid, i - row, column))
					grid[i - row][column].c_entity = NULL;
				else break;
			}
			else break;
		}
	}
}

entity* get_grid_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j)
{
	return grid[i][j].c_entity;
}

void set_initial_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl)
{
	grid[pl->left.row][pl->left.column].c_entity = pl->left.h_entity;
	grid[pl->right.row][pl->right.column].c_entity = pl->right.h_entity;
	grid[pl->left.row][pl->left.column].is_player = true;
	grid[pl->right.row][pl->right.column].is_player = true;
	set_grid_position_rect(get_rect(grid[pl->left.row][pl->left.column]), pl->left.row, pl->left.column);
	set_grid_position_rect(get_rect(grid[pl->right.row][pl->right.column]), pl->right.row, pl->right.column);
}

SDL_Rect* get_rect(cell c)
{
	return &c.c_entity->rect;
}

void set_grid_position_rect(SDL_Rect* rect, int i, int j)
{
	rect->x = (WINDOW_WIDTH / 2) - (GRID_ROWS * RECT_SIZE) + (PADDING + RECT_SIZE) * j;
	rect->y = (WINDOW_HEIGHT / 4) + (PADDING + RECT_SIZE) * i;
	rect->w = RECT_SIZE;
	rect->h = RECT_SIZE;
}

int check_cell_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j)
{
	if (grid[i][j].c_entity)
		return 1;
	return 0;
}

int check_cell_player(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j)
{
	return grid[i][j].is_player;
}

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r)
{
	if (check_left_collision(grid, pl, row_axis_l, column_axis_l) || check_right_collision(grid, pl, row_axis_r, column_axis_r))
		return 1;
	return 0;
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

void move_player_grid(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r)
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

int rotate_player_grid(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, rotation* rot, bool reverse)
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
	else move_player_grid(grid, pl, row_axis_l, column_axis_l, row_axis_r, column_axis_r);

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