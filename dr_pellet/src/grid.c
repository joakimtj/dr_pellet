#include <grid.h>

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

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r)
{
	if (check_left_collision(grid, pl, row_axis_l, column_axis_l) || check_right_collision(grid, pl, row_axis_r, column_axis_r))
		return 1;
	return 0;
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