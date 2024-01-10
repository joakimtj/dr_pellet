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
#define GRID_ROW_CLEARANCE 8 // Viruses will not spawn above or on this ROW.
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
	rotation roto;
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

SDL_Rect create_rect()
{
	#define DEFAULT_POS 0
	SDL_Rect rect = { DEFAULT_POS + (PADDING + RECT_SIZE), DEFAULT_POS + (PADDING + RECT_SIZE), RECT_SIZE, RECT_SIZE };
	return rect;
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

int check_cell_collision(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r) 
{
	int collisions = 0;
	if ((grid[pl->left.row + row_axis_l][pl->left.column + column_axis_l].c_entity) && !(grid[pl->left.row + row_axis_l][pl->left.column + column_axis_l].is_player))
	{
		puts("Left collision");
		collisions++;
	}
	if ((grid[pl->right.row + row_axis_r][pl->right.column + column_axis_r].c_entity) && !(grid[pl->right.row + row_axis_r][pl->right.column + column_axis_r].is_player))
	{
		puts("Right collision");
		collisions++;
	}
	return collisions;
}

void check_virus(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl) 
{
	// TODO: Implement this bro lol	
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

void move_player(cell grid[GRID_ROWS][GRID_COLUMNS], pill* pl, int row_axis_l, int column_axis_l, int row_axis_r, int column_axis_r) 
{
	// These temporary pointers allows the player blocks to move without respect to any order of operations.
	entity* tmp_1 = pl->left.h_entity;
	entity* tmp_2 = pl->right.h_entity;
	
	disable_pill(grid, pl);
	remove_pill(grid, pl);

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
	// These temporary pointers allows the player blocks to move without respect to any order of operations.
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

pill init_pill(entity* entities_p)
{
	half left = { &entities_p[0], 0, GRID_COLUMNS / 2 - 1 };
	half right = { &entities_p[1], 0, GRID_COLUMNS / 2 };

	return (pill) { left, right, FIRST, true };
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
	SDL_Rect bg_rect = create_rect();
	bg_rect.x = 0;
	bg_rect.y = 0;
	bg_rect.w = WINDOW_WIDTH;
	bg_rect.h = WINDOW_HEIGHT;
	SDL_SetRenderDrawColor(renderer, 0x8b, 0xb4, 0xd9, 255);
	SDL_RenderFillRect(renderer, &bg_rect);
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

	pill pl = init_pill(entities_p);
	set_initial_pill(grid, &pl);

	int pill_count = 2;

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
	uint32_t frame_count = 0;
	uint32_t last_second = previous_time;
	float step_time = 0;
	float step_limit = 1;

	int gravity = 1;
	bool disable_ghost = false;
	bool pause = false;
	
	int ret = 0;

	rotation rot = FIRST;

	speed fall_speed = NORMAL;
	direction dir_x = NEUTRAL;

	bool quit = false;
	while (!quit)
	{
		current_time = SDL_GetTicks();
		delta_time = (current_time - previous_time) / 1000.0f;
		previous_time = current_time;

		if (pause) delta_time = 0;
		step_time += delta_time;

		frame_count++;
		if (current_time - last_second >= 1000) {
			printf("FPS: %d\n", frame_count);
			frame_count = 0;
			last_second = current_time;
		}

		while (SDL_PollEvent(&event))
		{	
			if (event.key.keysym.sym == SDLK_e && event.key.state == SDL_PRESSED) {
				if (rot > 3) rot = 0;
				ret = rotate_player(grid, &pl, &rot, false);
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

			if (event.key.keysym.sym == SDLK_r && event.key.state == SDL_PRESSED) {
				rot -= 1;
				if (rot < 0) rot = 3;
				ret = rotate_player(grid, &pl, &rot, true);
				if (ret != 0) {
					puts("Can't move.");
					rot++;
				}
				else {
					set_player_position_rect(grid, &pl);
					printf("Left: i=%d j=%d\n", pl.left.row, pl.left.column);
					printf("Right: i=%d j=%d\n", pl.right.row, pl.right.column);	
				}
			}

			if (event.key.keysym.sym == SDLK_s && event.key.state == SDL_PRESSED) {
				step_time = 0;
				set_player_position_rect(grid, &pl);
				if (!(check_cell_collision(grid, &pl, gravity, NEUTRAL, gravity, NEUTRAL)) && ((pl.left.row + gravity < GRID_ROWS) && pl.right.row + gravity < GRID_ROWS))
				{
					move_player(grid, &pl, gravity, 0, gravity, 0);
					set_player_position_rect(grid, &pl);
				}
				else
				{
					check_virus(grid, &pl);
					pl.active = false;
					printf("Key-S: Collision.\n");
				}
			}

			if (event.key.keysym.sym == SDLK_a && event.key.state == SDL_PRESSED) {
				dir_x = LEFT;
			}

			if (event.key.keysym.sym == SDLK_d && event.key.state == SDL_PRESSED) {
				dir_x = RIGHT;
			}

			if (event.key.keysym.sym == SDLK_ESCAPE && event.key.state == SDL_PRESSED) {
				switch (pause)
				{
					case true:
					pause = false;
					break;
					case false:
					pause = true;
					break;
				}
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
		if (!pl.active) 
		{
			pill_count += 2;
			
			disable_pill(grid, &pl);

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

			if (check_cell_collision(grid, &pl, pl.left.row, pl.left.column, pl.right.row, pl.right.column)) 
			{
				quit = true;
				printf("Oh no... ! You lost !\n");
			}
			else 
			{
				move_player(grid, &pl, 0, 0, 0, 0);
				set_player_position_rect(grid, &pl);

				pl.active = true;
			}
		}

		if (dir_x) 
		{
			if (dir_x == LEFT) 
			{
				if (!(check_cell_collision(grid, &pl, 0, LEFT, 0, LEFT)))
				{
					if (pl.left.column + dir_x >= 0 && pl.right.column + dir_x >= 0)
					{
						move_player(grid, &pl, 0, dir_x, 0, dir_x);
						set_player_position_rect(grid, &pl);
					}
				}
			}
			if (dir_x == RIGHT)
			{
				if (!(check_cell_collision(grid, &pl, 0, RIGHT, 0, RIGHT)))
				{
					if (pl.left.column + dir_x < GRID_COLUMNS && pl.right.column + dir_x < GRID_COLUMNS)
					{
						move_player(grid, &pl, 0, dir_x, 0, dir_x);
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
			if (!(check_cell_collision(grid, &pl, gravity, NEUTRAL, gravity, NEUTRAL)) && ((pl.left.row + gravity < GRID_ROWS) && pl.right.row + gravity < GRID_ROWS))
			{
				move_player(grid, &pl, gravity, 0, gravity, 0);
				set_player_position_rect(grid, &pl);
			}
			else 
			{
				check_virus(grid, &pl);
				pl.active = false;
				printf("Step-time Collision.\n");
			}
			step_time = 0;
		}

		// Rendering
		SDL_RenderClear(renderer);
		
		render_bg(renderer);

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
	}
	free(entities_v);
	free(entities_p);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}