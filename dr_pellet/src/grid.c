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

int check_cell_type(cell grid[GRID_ROWS][GRID_COLUMNS], int i, int j, entity_type type)
{
	if (grid[i][j].c_entity->type == type)
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

void delete_cell_entity(cell grid[GRID_ROWS][GRID_COLUMNS], int row, int column)
{
	grid[row][column].c_entity = NULL;
}

int check_entities_below(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column)
{
	int total_below = 0;

	int to_check = 1;
	for (int i = row; i < i + to_check; i++)
	{
		if (i < GRID_ROWS - 1)
		{
			if (check_cell_entity(grid, i + 1, column))
			{
				if (check_cell_type(grid, i + 1, column, get_cell_type(grid, row, column)) || check_cell_type(grid, i + 1, column, get_cell_type(grid, row, column) - 3))
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
	return total_below;
}

int check_entities_above(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column)
{
	int total_above = 0;

	int to_check = 1;
	for (int i = row; i > i - to_check; i--)
	{
		if (i > 0)
		{
			if (check_cell_entity(grid, i - 1, column))
			{
				if (check_cell_type(grid, i - 1, column, get_cell_type(grid, row, column)) || check_cell_type(grid, i - 1, column, get_cell_type(grid, row, column) - 3))
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
	return total_above;
}

int check_entities_right(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column)
{
	int total_right = 0;

	int to_check = 1;
	for (int i = column; i < i + to_check; i++)
	{
		if (i < GRID_COLUMNS - 1)
		{
			if (check_cell_entity(grid, row, i + 1))
			{
				if (check_cell_type(grid, row, i + 1, get_cell_type(grid, row, column)) || check_cell_type(grid, row, i + 1, get_cell_type(grid, row, column) - 3))
				{
					to_check++;
					total_right++;
				}
				else break;
			}
			else break;
		}
		else break;
	}
	return total_right;
}

int check_entities_left(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column)
{
	int total_left = 0;

	int to_check = 1;
	for (int i = column; i > i - to_check; i--)
	{
		if (i > 0)
		{
			if (check_cell_entity(grid, row, i - 1))
			{
				if (check_cell_type(grid, row, i - 1, get_cell_type(grid, row, column)) || check_cell_type(grid, row, i - 1, get_cell_type(grid, row, column) - 3))
				{
					to_check++;
					total_left++;
				}
				else break;
			}
			else break;
		}
		else break;
	}
	return total_left;
}

void delete_entities_below(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column, int total_below)
{
	for (int i = 1; i <= total_below; i++)
	{
		if (i + row <= GRID_ROWS - 1)
		{
			if (check_cell_entity(grid, i + row, column))
				grid[i + row][column].c_entity = NULL;
			else break;
		}
		else break;
	}
}

void delete_entities_above(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column, int total_above)
{
	for (int i = 1; i <= total_above; i++)
	{
		if (i + row >= 0)
		{
			if (check_cell_entity(grid, -i + row, column))
				grid[-i + row][column].c_entity = NULL;
			else break;
		}
		else break;
	}
}

void delete_entities_right(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column, int total_right)
{
	for (int i = 1; i <= total_right; i++)
	{
		if (i + column <= GRID_COLUMNS - 1)
		{
			if (check_cell_entity(grid, row, i + column))
				grid[row][i + column].c_entity = NULL;
			else break;
		}
		else break;
	}
}

void delete_entities_left(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column, int total_left)
{
	for (int i = 1; i <= total_left; i++)
	{
		if (i >= 0)
		{
			if (check_cell_entity(grid, row, -i + column))
				grid[row][-i + column].c_entity = NULL;
			else break;
		}
		else break;
	}
}

void clear_viruses(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row, int column)
{
	int has_cleared = 0;

	int total_below = 0;
	int total_above = 0;
	int total_right = 0;
	int total_left = 0;

	int total_vertical_sum = 0;
	int total_horizontal_sum = 0;

	total_below = check_entities_below(grid, pl, row, column);

	total_above = check_entities_above(grid, pl, row, column);

	total_right = check_entities_right(grid, pl, row, column);

	total_left = check_entities_left(grid, pl, row, column);

	total_vertical_sum = total_below + total_above + 1; // +1 refers to the player controlled pill that connected with whatever entity in the grid.
	total_horizontal_sum = total_right + total_left + 1;
	printf("Total below: %d\n", total_below);
	printf("Total above: %d\n", total_above);
	printf("Total right: %d\n", total_right);
	printf("Total left: %d\n", total_left);
	printf("Sum of V total: %d\n", total_vertical_sum);
	printf("Sum of H total: %d\n", total_horizontal_sum);
	if (total_vertical_sum > 3) {
		delete_entities_below(grid, pl, row, column, total_below);
		delete_entities_above(grid, pl, row, column, total_above);
		
		delete_cell_entity(grid, row, column);
	}

	if (total_horizontal_sum > 3) {
		delete_entities_right(grid, pl, row, column, total_right);
		delete_entities_left(grid, pl, row, column, total_left);

		delete_cell_entity(grid, row, column);
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
	float xpos = offset + GRID_POSITION_X + RECT_SIZE * j;
	rect->x = (int)xpos;
	float ypos = offset + GRID_POSITION_Y + RECT_SIZE * i;
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

int check_left_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_L, int column_L)
{
	if ((grid[pl->left.row + row_L][pl->left.column + column_L].c_entity) && !(grid[pl->left.row + row_L][pl->left.column + column_L].is_player))
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
		set_pill_active(pl, false);
		clear_viruses(grid, pl, get_left_row(pl), get_left_column(pl));
	}

	if (check_right_collision(grid, pl, gravity, 0))
	{
		set_pill_active(pl, false);
		clear_viruses(grid, pl, get_right_row(pl), get_right_column(pl));
	}

	if (get_pill_active(pl))
	{
		if ((check_grid_index_bounds(get_left_row(pl) + gravity)) && check_grid_index_bounds(get_right_row(pl) + gravity))
		{	
			move_player_grid(grid, pl, gravity, 0, gravity, 0);
			set_player_position_rect(grid, pl);
		}
		else
		{
			set_pill_active(pl, false);
			clear_viruses(grid, pl, get_left_row(pl), get_left_column(pl));
			clear_viruses(grid, pl, get_right_row(pl), get_right_column(pl));
		}
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
