#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_rect.h>
#include <SDL_timer.h>

#define TITLE "Dr. Pellet"
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800 
#define GRID_ROWS 12 // The height of the play-area.
#define GRID_COLUMNS 8 // The width of the play-area.
#define GRID_ROW_CLEARANCE 6 // Viruses will not spawn above or on this ROW.
#define VIRUS_N 66 // The N viruses to be spawned on the grid.
#define RECT_SIZE 30
#define PADDING 1

enum {
	SLOW,
	NORMAL,
	FAST,
	VERY_FAST
} typedef speed;

enum {
	FIRST,
	SECOND,
	THIRD,
	FOURTH
} typedef rotation;

enum {
	LEFT = -1,
	NEUTRAL = 0,
	RIGHT = 1
} typedef direction;

enum {
	RED_VIRUS,
	BLUE_VIRUS,
	YELLOW_VIRUS,
	RED_BLOCK,
	BLUE_BLOCK,
	YELLOW_BLOCK
} typedef entity_type;

struct {
	entity_type type;
	SDL_Rect rect;
} typedef entity;

struct {
	entity* c_entity;
	bool is_player;
} typedef cell;

struct {
	entity* h_entity;
	int row;
	int column;
} typedef half;

struct {
	half left;
	half right;
	bool active;
} typedef pill;

int get_random_integer(int i) 
{
	return rand() % i;
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

void set_player_position_rect(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl) 
{
	set_grid_position_rect(get_rect(grid[pl->left.row][pl->left.column]), pl->left.row, pl->left.column);
	set_grid_position_rect(get_rect(grid[pl->right.row][pl->right.column]), pl->right.row, pl->right.column);
}

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis, int column_axis) 
{
	// Checks if the adjacent node (selected by row_axis and column_axis) is occupied by a cell and if that cell is the player.
	if ((grid[pl->left.row + row_axis][pl->left.column + column_axis].c_entity) && !(grid[pl->left.row + row_axis][pl->left.column + column_axis].is_player) || 
		(grid[pl->right.row + row_axis][pl->right.column + column_axis].c_entity) && !(grid[pl->right.row + row_axis][pl->right.column + column_axis].is_player))
		return 1;
	else return 0;
}


void move_player(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis, int column_axis) 
{
	// These temporary pointers allows the player blocks to move without respect to any order of operations.
	entity* tmp_1 = pl->left.h_entity;
	entity* tmp_2 = pl->right.h_entity;
	
	grid[pl->left.row][pl->left.column].is_player = false;
	grid[pl->right.row][pl->right.column].is_player = false;

	grid[pl->left.row][pl->left.column].c_entity = NULL;
	grid[pl->right.row][pl->right.column].c_entity = NULL;

	grid[pl->left.row + row_axis][pl->left.column + column_axis].is_player = true;
	grid[pl->right.row + row_axis][pl->right.column + column_axis].is_player = true;

	grid[pl->left.row + row_axis][pl->left.column + column_axis].c_entity = tmp_1;
	grid[pl->right.row + row_axis][pl->right.column + column_axis].c_entity = tmp_2;

	pl->left.h_entity = tmp_1;
	pl->right.h_entity = tmp_2;
}

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
					random_i = get_random_integer(5);
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

SDL_Rect create_rect() 
{
	#define DEFAULT_POS 0	
	SDL_Rect rect = { DEFAULT_POS + (PADDING + RECT_SIZE), DEFAULT_POS + (PADDING + RECT_SIZE), RECT_SIZE, RECT_SIZE};
	return rect;
}

entity* init_entities(int mode, int n_entities) 
{	/*	
		@param int mode : 0 or 3. Determines which range of types to create. Virus or Block.
	*/
	int RED_COUNT = 0, BLUE_COUNT = 0, YELLOW_COUNT = 0;
	if (mode != 0 && mode != 3) {
		printf("Mode was %d, expected 0 or 3.+\n", mode);
		return -1;
	}
	entity* entities = malloc(sizeof(entity) * n_entities);
	entity_type e_type;
	for (int i = 0; i < n_entities; i++)
	{
		e_type = (rand() % 3 + mode);
		switch (e_type) 
		{
			case RED_VIRUS:
				RED_COUNT++;
				break;
			case BLUE_VIRUS:
				BLUE_COUNT++;
				break;
			case YELLOW_VIRUS:
				YELLOW_COUNT++;
				break;
		}
		entity ent = { e_type, create_rect() };
		entities[i] = ent;
	}
	return entities;
}

void render_character_area(SDL_Renderer* renderer, SDL_Texture* dr_pellet_t) 
{
	SDL_Rect area_rect;
	area_rect.x = WINDOW_WIDTH / 2;
	area_rect.y = WINDOW_HEIGHT / 3.5;
	area_rect.w = 200;
	area_rect.h = 200;

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 200);
	SDL_RenderFillRect(renderer, &area_rect);
	SDL_RenderDrawRect(renderer, &area_rect);

	// White Outline
	SDL_Rect outline_rect;
	outline_rect.x = -1 + WINDOW_WIDTH / 2;
	outline_rect.y = -1 + WINDOW_HEIGHT / 3.5;
	outline_rect.w = 200 + 2;
	outline_rect.h = 200 + 2;

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 255);
	SDL_RenderDrawRect(renderer, &outline_rect);

	SDL_Rect character_rect;
	character_rect.x = 60 + WINDOW_WIDTH / 2;
	character_rect.y = 20 + WINDOW_HEIGHT / 3.5;
	character_rect.w = 40 * 2;
	character_rect.h = 86 * 2;

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
	SDL_RenderDrawRect(renderer, &character_rect);

	SDL_RenderCopy(renderer, dr_pellet_t, NULL, &character_rect);
}

void render_grid_area(SDL_Renderer* renderer) 
{
	SDL_Rect grid_rect;
	grid_rect.x = WINDOW_WIDTH / 5;
	grid_rect.y = WINDOW_HEIGHT / 4;
	grid_rect.w = GRID_COLUMNS * RECT_SIZE + PADDING + 6;
	grid_rect.h = GRID_ROWS * RECT_SIZE + PADDING + 10;

	SDL_SetRenderDrawColor(renderer, 0x00, 0x0, 0x00, 200);
	SDL_RenderFillRect(renderer, &grid_rect);
	SDL_RenderDrawRect(renderer, &grid_rect);

	// White Outline
	SDL_Rect outline_rect;
	outline_rect.x = -1 + WINDOW_WIDTH / 5;
	outline_rect.y = -1 + WINDOW_HEIGHT / 4;
	outline_rect.w = GRID_COLUMNS * RECT_SIZE + PADDING + 6 + 2;
	outline_rect.h = GRID_ROWS * RECT_SIZE + PADDING + 10 + 2;

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 255);
	SDL_RenderDrawRect(renderer, &outline_rect);
	
}

void render_bg(SDL_Renderer* renderer) 
{
	for (int i = 0; i <= WINDOW_HEIGHT / RECT_SIZE; i++)
	{
		for (int j = 0; j <= WINDOW_WIDTH / RECT_SIZE; j++)
		{
			SDL_Rect bg_rect = create_rect();
			bg_rect.x = ( -16 + RECT_SIZE * j);
			bg_rect.y = -2 + RECT_SIZE * i;
			bg_rect.w = RECT_SIZE;
			bg_rect.h = RECT_SIZE;

			if (j % 2 == 0) 
			{
				SDL_SetRenderDrawColor(renderer, 0x8b, 0xb4, 0xd9, 255);
				SDL_RenderFillRect(renderer, &bg_rect);
				SDL_RenderDrawRect(renderer, &bg_rect);
			}
			else if (i % 2 == 0) 
			{
				SDL_SetRenderDrawColor(renderer, 0x8b, 0xb4, 0xd9, 255);
				SDL_RenderFillRect(renderer, &bg_rect);
				SDL_RenderDrawRect(renderer, &bg_rect);
			}
			else 
			{
				SDL_SetRenderDrawColor(renderer, 0x8b, 0xd9, 0xd2, 255);
				SDL_RenderFillRect(renderer, &bg_rect);
				SDL_RenderDrawRect(renderer, &bg_rect);
			}
		}
	}
}

int rotate_player(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, rotation* rot) 
{
	// These temporary pointers allows the player blocks to move without respect to any order of operations.
	int row_axis_l = 0, column_axis_l = 0;
	int row_axis_r = 0, column_axis_r = 0;
	switch (*rot) 
	{
		case FIRST:
		{
			row_axis_l = -1;
			column_axis_r = -1;
		}
		break;

		case SECOND:
		{
			row_axis_l = 1;
			column_axis_l = 1;
		}
		break;

		case THIRD:
		{
			column_axis_l = -1;
			row_axis_r = -1;
		}
		break;

		case FOURTH:
		{
			row_axis_r = 1;
			column_axis_r = 1;
		}
		break;
	}

	entity* tmp_1 = pl->left.h_entity;
	entity* tmp_2 = pl->right.h_entity;

	if (pl->left.row + row_axis_l < 0 || pl->right.row + row_axis_r < 0) {
		return -1;
	}
	else {
		if (pl->left.column + column_axis_l >= GRID_COLUMNS - 1 && pl->right.column + column_axis_r >= GRID_COLUMNS - 1) {
			column_axis_l--;
			column_axis_r--;
		}

		if ((grid[pl->left.row + row_axis_l][pl->left.column + column_axis_l].c_entity) && !(grid[pl->left.row + row_axis_l][pl->left.column + column_axis_l].is_player) ||
			(grid[pl->right.row + row_axis_r][pl->right.column + column_axis_r].c_entity) && !(grid[pl->right.row + row_axis_r][pl->right.column + column_axis_r].is_player))
		{
			return -1;
		}

		grid[pl->left.row][pl->left.column].is_player = false;
		grid[pl->right.row][pl->right.column].is_player = false;

		grid[pl->left.row][pl->left.column].c_entity = NULL;
		grid[pl->right.row][pl->right.column].c_entity = NULL;

		grid[pl->left.row + row_axis_l][pl->left.column + column_axis_l].is_player = true;
		grid[pl->right.row + row_axis_r][pl->right.column + column_axis_r].is_player = true;

		grid[pl->left.row + row_axis_l][pl->left.column + column_axis_l].c_entity = tmp_1;
		grid[pl->right.row + row_axis_r][pl->right.column + column_axis_r].c_entity = tmp_2;

		pl->left.row = pl->left.row + row_axis_l;
		pl->left.column = pl->left.column + column_axis_l;
		pl->right.row = pl->right.row + row_axis_r;
		pl->right.column = pl->right.column + column_axis_r;

		pl->left.h_entity = tmp_1;
		pl->right.h_entity = tmp_2;
	}
	return 0;
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_Event event;

	srand(time(NULL));

	entity* entities_v = init_entities(0, VIRUS_N);
	entity* entities_p = init_entities(0, 128 * 2);

	cell grid[GRID_ROWS][GRID_COLUMNS];
	init_grid(grid, entities_v);

	half left = { &entities_p[0], 0, 3 };
	half right = { &entities_p[1], 0, 4 };

	pill pl = { left, right, true};
	int pill_count = 2;

	// Init first player block 
	grid[pl.left.row][pl.left.column].c_entity = pl.left.h_entity;
	grid[pl.right.row][pl.right.column].c_entity = pl.right.h_entity;
	grid[pl.left.row][pl.left.column].is_player = true;
	grid[pl.right.row][pl.right.column].is_player = true;
	set_grid_position_rect(get_rect(grid[pl.left.row][pl.left.column]), pl.left.row, pl.left.column);
	set_grid_position_rect(get_rect(grid[pl.right.row][pl.right.column]), pl.right.row, pl.right.column);

	SDL_Surface* red_block_bmp = SDL_LoadBMP("red_block.bmp");
	SDL_Texture* red_block_t = SDL_CreateTextureFromSurface(renderer, red_block_bmp);
	SDL_FreeSurface(red_block_bmp);

	SDL_Surface* blue_block_bmp = SDL_LoadBMP("blue_block.bmp");
	SDL_Texture* blue_block_t = SDL_CreateTextureFromSurface(renderer, blue_block_bmp);
	SDL_FreeSurface(blue_block_bmp);

	SDL_Surface* yellow_block_bmp = SDL_LoadBMP("yellow_block.bmp");
	SDL_Texture* yellow_block_t = SDL_CreateTextureFromSurface(renderer, yellow_block_bmp);
	SDL_FreeSurface(yellow_block_bmp);

	SDL_Surface* bg_bmp = SDL_LoadBMP("bg.bmp");
	SDL_Texture* bg_t = SDL_CreateTextureFromSurface(renderer, bg_bmp);
	SDL_FreeSurface(bg_bmp);

	SDL_Surface* dr_pellet_bmp = SDL_LoadBMP("dr_pellet.bmp");
	SDL_Texture* dr_pellet_t = SDL_CreateTextureFromSurface(renderer, dr_pellet_bmp);
	SDL_FreeSurface(dr_pellet_bmp);

	uint32_t previous_time = SDL_GetTicks();
	uint32_t current_time;
	float delta_time;

	float step_time = 0;
	float step_limit = 1;

	int gravity = 1;
	bool disable_ghost = false;

	rotation rot = FIRST;

	speed fall_speed = NORMAL;
	direction dir_x = NEUTRAL;

	bool quit = false;
	while (!quit)
	{
		current_time = SDL_GetTicks();
		delta_time = (current_time - previous_time) / 1000.0f;
		step_time += delta_time;

		while (SDL_PollEvent(&event))
		{	
			if (event.key.keysym.sym == SDLK_UP && event.key.state == SDL_PRESSED) {
				if (rot > 3) rot = 0;
				int ret = rotate_player(grid, &pl, &rot);
				if (ret != 0) {
					puts("Can't move.");
				}
				else {
					set_player_position_rect(grid, &pl);
					printf("Left: i=%d j=%d\n", pl.left.row, pl.left.column);
					printf("Right: i=%d j=%d\n", pl.right.row, pl.right.column);
					rot += 1;
				}
				
			}

			if (event.key.keysym.sym == SDLK_DOWN && event.key.state == SDL_PRESSED) {
				switch (fall_speed) 
				{
					case NORMAL:
						step_limit = 0.5;
						fall_speed = SLOW;
						break;
					case SLOW:
						step_limit = 1;
						fall_speed = NORMAL;
						break;
				}
			}

			if (event.key.keysym.sym == SDLK_LEFT && event.key.state == SDL_PRESSED) {
				dir_x = LEFT;
			}

			if (event.key.keysym.sym == SDLK_RIGHT && event.key.state == SDL_PRESSED) {
				dir_x = RIGHT;
			}

			if (event.key.keysym.sym == SDLK_g && event.key.state == SDL_PRESSED) {
				switch (disable_ghost)
				{
					case true:
						disable_ghost = false;
						break;
					case false:
						disable_ghost = true;
						break;
				}
			}

			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}


		// Logic

		// Sets new active pill
		if (!pl.active && step_time < step_limit) 
		{
			pill_count += 2;

			grid[pl.left.row][pl.left.column].is_player = false;
			grid[pl.right.row][pl.right.column].is_player = false;

			entity* tmp_1;
			entity* tmp_2;
			
			int difference_left = pl.left.row - pl.right.row;
			int difference_right = pl.right.row - pl.left.row;

			if (difference_left > 0) {
				pl.left.row = difference_left;
			}
			else {
				pl.left.row = 0;
			}

			if (difference_right > 0) {
				pl.right.row = difference_right;
			}
			else {
				pl.right.row = 0;
			}

			pl.left.h_entity = &entities_p[pill_count];
			pl.right.h_entity = &entities_p[pill_count + 1];

			tmp_1 = pl.left.h_entity;
			tmp_2 = pl.right.h_entity;

			if (check_cell_collision(grid, &pl, 0, 0)) 
			{
				quit = true;
				printf("Oh no... ! You lost !\n");
			}
			else 
			{
				grid[pl.right.row][pl.right.column].c_entity = tmp_2;
				grid[pl.left.row][pl.left.column].c_entity = tmp_1;

				grid[pl.left.row][pl.left.column].is_player = true;
				grid[pl.right.row][pl.right.column].is_player = true;

				set_player_position_rect(grid, &pl);

				pl.active = true;
			}
		}

		if (dir_x) 
		{
			if (dir_x == LEFT) 
			{
				if (!(check_cell_collision(grid, &pl, 0, LEFT)))
				{
					if (pl.left.column + dir_x >= 0 && pl.right.column + dir_x >= 0)
					{
						move_player(grid, &pl, 0, dir_x);

						pl.left.column = pl.left.column + dir_x;
						pl.right.column = pl.right.column + dir_x;

						set_player_position_rect(grid, &pl);
					}
				}
			}
			if (dir_x == RIGHT)
			{
				if (!(check_cell_collision(grid, &pl, 0, RIGHT)))
				{
					if (pl.left.column + dir_x < GRID_COLUMNS && pl.right.column + dir_x < GRID_COLUMNS)
					{
						move_player(grid, &pl, 0, dir_x);

						pl.left.column = pl.left.column + dir_x;
						pl.right.column = pl.right.column + dir_x;

						set_player_position_rect(grid, &pl);
					}
				}
			}
			dir_x = NEUTRAL;
		}

		// Gravity is applied here.
		if (step_time > step_limit && pl.active)
		{
			set_player_position_rect(grid, &pl);
			if (!(check_cell_collision(grid, &pl, gravity, NEUTRAL)))
			{
				move_player(grid, &pl, 1, 0);

				pl.left.row++;
				pl.right.row++;

				set_player_position_rect(grid, &pl);
			}
			else 
			{
				pl.active = false;
				printf("Step-time Collision.\n");
			}
			step_time = 0;
		}

		// Rendering
		SDL_RenderClear(renderer);
		
		render_bg(renderer, delta_time);

		render_grid_area(renderer);

		render_character_area(renderer, dr_pellet_t);

		bool render_ghost = false;
		for (int i = 0; i < GRID_ROWS; i++) 
		{
			for (int j = 0; j < GRID_COLUMNS; j++) 
			{
				// Virus greier 
				if (grid[i][j].c_entity != NULL) 
				{	
					if (grid[i][j].c_entity->type == RED_VIRUS) {
						SDL_RenderCopy(renderer, red_block_t, NULL, get_rect(grid[i][j]));
					}

					if (grid[i][j].c_entity->type == BLUE_VIRUS) {
						SDL_RenderCopy(renderer, blue_block_t, NULL, get_rect(grid[i][j]));
					}
		
					if (grid[i][j].c_entity->type == YELLOW_VIRUS) {
						SDL_RenderCopy(renderer, yellow_block_t, NULL, get_rect(grid[i][j]));
					}
				}
				
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);
		previous_time = current_time;
	}
	free(entities_v);
	free(entities_p);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}