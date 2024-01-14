#include "grid.h"

void init_grid(cell grid[GRID_ROWS][GRID_COLUMNS], entity* entities)
{
	int entity_count = 0;
	for (int i = 0; i < GRID_ROWS; i++)
	{
		for (int j = 0; j < GRID_COLUMNS; j++)
		{
			if (i > GRID_ROW_CLEARANCE)
			{
				if (entity_count < VIRUS_N)
				{
					int random_i = get_random_integer(2);
					if (random_i == 0) {
						grid[i][j].c_entity = NULL;
					}
					else {
						grid[i][j].c_entity = &entities[entity_count];
						set_grid_position_rect(&grid[i][j].c_entity->rect, i, j, 0);
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
}

int check_cell_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j)
{
	if (grid[i][j].c_entity)
		return 1;
	return 0;
}

entity* get_cell_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j)
{
	return grid[i][j].c_entity;
}

entity_type get_cell_type(cell grid[GRID_ROWS][GRID_COLUMNS], int row, int column)
{
	return grid[row][column].c_entity->type;
}

void clear_virus_vertical(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column)
{
	int total_below = 0;
	int total_above = -1;
	int total_sum = 0;

	int to_check = 1;
	for (int i = row; i < i + to_check; i++)
	{
		if (i < GRID_ROWS - 1)
		{
			if (check_cell_entity(grid, i, column))
			{
				if (get_cell_type(grid, row, column) == get_cell_type(grid, i, column) || (get_cell_type(grid, row, column) - 3) == get_cell_type(grid, i, column))
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
				if (grid[row][column].c_entity->type == grid[i][column].c_entity->type || (grid[row][column].c_entity->type - 3) == grid[i][column].c_entity->type)
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

void set_initial_pill(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl)
{
	grid[pl->left.row][pl->left.column].c_entity = pl->left.h_entity;
	grid[pl->right.row][pl->right.column].c_entity = pl->right.h_entity;
	grid[pl->left.row][pl->left.column].is_player = true;
	grid[pl->right.row][pl->right.column].is_player = true;
	set_grid_position_rect(get_rect(grid[pl->left.row][pl->left.column]), pl->left.row, pl->left.column, 0);
	set_grid_position_rect(get_rect(grid[pl->right.row][pl->right.column]), pl->right.row, pl->right.column, 0);
}

SDL_Rect* get_rect(cell c)
{
	return &c.c_entity->rect;
}

void set_grid_position_rect(SDL_Rect* rect, int i, int j, int offset)
{
	float xpos = offset + (WINDOW_WIDTH / 1.5f) - (GRID_ROWS * RECT_SIZE) + RECT_SIZE * j;
	rect->x = (int)xpos;
	float ypos = offset + (WINDOW_HEIGHT / 5.5f) + RECT_SIZE * i;
	rect->y = (int) ypos;
	rect->w = RECT_SIZE;
	rect->h = RECT_SIZE;
}

void interpolate_grid_position_rect(SDL_Rect* rect, int start_i, int end_i, int j, float interp_factor)
{
	if (end_i > GRID_ROWS) return -1;
	// Calculate the starting and ending y positions
	int start_y = (WINDOW_HEIGHT / 4) + RECT_SIZE * start_i;
	int end_y = (WINDOW_HEIGHT / 4) + RECT_SIZE * end_i;

	// Set x position (remains the same as in set_grid_position_rect)
	rect->x = (WINDOW_WIDTH / 2) - (GRID_ROWS * RECT_SIZE) + RECT_SIZE * j;

	// Interpolate y position
	rect->y = start_y + (int)((end_y - start_y) * interp_factor);

	// Set width and height
	rect->w = RECT_SIZE;
	rect->h = RECT_SIZE;
}

int check_grid_index_bounds(int i)
{
	if (i < GRID_ROWS)
		return 1;
	return 0;
}

int check_cell_player(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j)
{
	return grid[i][j].is_player;
}

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_L, int column_L, int row_R, int column_R)
{
	if (check_left_collision(grid, pl, row_L, column_L) || check_right_collision(grid, pl, row_R, column_R))
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
	set_grid_position_rect(get_rect(grid[pl->left.row][pl->left.column]), pl->left.row, pl->left.column, 0);
	set_grid_position_rect(get_rect(grid[pl->right.row][pl->right.column]), pl->right.row, pl->right.column, 0);
}

void move_player_grid(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_L, int column_L, int row_R, int column_R)
{
	// These temporary pointers allows the player blocks to move without respect to any order of operations.
	entity* tmp_1 = pl->left.h_entity;
	entity* tmp_2 = pl->right.h_entity;

	remove_pill(grid, pl);
	disable_pill(grid, pl);

	pl->left.row = pl->left.row + row_L;
	pl->left.column = pl->left.column + column_L;
	pl->right.row = pl->right.row + row_R;
	pl->right.column = pl->right.column + column_R;

	enable_pill(grid, pl);

	grid[pl->left.row][pl->left.column].c_entity = tmp_1;
	grid[pl->right.row][pl->right.column].c_entity = tmp_2;

	pl->left.h_entity = tmp_1;
	pl->right.h_entity = tmp_2;
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

void update_player_grid(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int gravity)
{
	if (check_left_collision(grid, pl, gravity, 0))
	{
		clear_virus_vertical(grid, pl, get_left_row(pl), get_left_column(pl));
		set_pill_active(pl, false);
	}

	if (check_right_collision(grid, pl, gravity, 0))
	{
		clear_virus_vertical(grid, pl, get_right_row(pl), get_right_column(pl));
		set_pill_active(pl, false);
	}

	if (get_pill_active(pl))
	{
		if ((check_grid_index_bounds(get_left_row(pl) + gravity)) && check_grid_index_bounds(get_right_row(pl) + gravity))
		{
			move_player_grid(grid, pl, gravity, 0, gravity, 0);
			set_player_position_rect(grid, pl);
		}
		else set_pill_active(pl, false);
	}
}

int rotate_player_grid(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, bool reverse)
{
	/*
	* A pill has four rotational states, FIRST, SECOND, THIRD and FOURTH.
	* These require their own bespoke transformations.
	*/

	int row_L = 0; int column_L = 0;
	int row_R = 0; int column_R = 0;

	direction r_state = get_rotation_state(pl);
	switch (r_state) {
		case FIRST: { row_L = -1; column_R = -1; } break;

		case SECOND: { row_L = 1; column_L = 1; } break;

		case THIRD: { column_L = -1; row_R = -1; } break;

		case FOURTH: { row_R = 1; column_R = 1; } break;
	}

	if (reverse) {
		row_L = -row_L; column_L = -column_L;
		row_R = -row_R; column_R = -column_R;
	}
	// Wall kick off ceiling
	if (get_left_row(pl) + row_L < 0 || get_right_row(pl) + row_R < 0) {
		row_L++; row_R++;
	}
	// Wall kick off right wall
	if (get_left_column(pl) + column_L >= GRID_COLUMNS - 1 && get_right_column(pl) + column_R >= GRID_COLUMNS - 1) {
		column_L--; column_R--;
	}
	
	if (check_cell_collision(grid, pl, row_L, column_L, row_R, column_R)) return -1;
	else move_player_grid(grid, pl, row_L, column_L, row_R, column_R);

	return 0;
}
