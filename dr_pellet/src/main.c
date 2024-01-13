#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "util.h"
#include "render.h"
#include "entity.h"
#include "player.h"
#include "grid.h"

#include <SDL.h>
#include <SDL_rect.h>
#include <SDL_timer.h>

typedef enum {
	SLOW,
	NORMAL,
	FAST,
	VERY_FAST
} speed;

typedef enum  {
	LEFT = -1,
	NEUTRAL = 0,
	RIGHT = 1
} direction;

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

	SDL_Surface* red_block_bmp = SDL_LoadBMP("assets/red_block.bmp");
	SDL_Texture* red_block_t = SDL_CreateTextureFromSurface(renderer, red_block_bmp);
	SDL_FreeSurface(red_block_bmp);

	SDL_Surface* blue_block_bmp = SDL_LoadBMP("assets/blue_block.bmp");
	SDL_Texture* blue_block_t = SDL_CreateTextureFromSurface(renderer, blue_block_bmp);
	SDL_FreeSurface(blue_block_bmp);

	SDL_Surface* yellow_block_bmp = SDL_LoadBMP("assets/yellow_block.bmp");
	SDL_Texture* yellow_block_t = SDL_CreateTextureFromSurface(renderer, yellow_block_bmp);
	SDL_FreeSurface(yellow_block_bmp);

	SDL_Surface* bg_bmp = SDL_LoadBMP("assets/bg.bmp");
	SDL_Texture* bg_t = SDL_CreateTextureFromSurface(renderer, bg_bmp);
	SDL_FreeSurface(bg_bmp);

	SDL_Surface* dr_pellet_bmp = SDL_LoadBMP("assets/dr_pellet.bmp");
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
	int return_left = 0;
	int return_right = 0;


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

		/*
		frame_count++;
		if (current_time - last_second >= 1000) {
			printf("FPS: %d\n", frame_count);
			frame_count = 0;
			last_second = current_time;
		}
		*/
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
				}
			}

			if (event.key.keysym.sym == SDLK_s && event.key.state == SDL_PRESSED) {
				step_time = 0;
				set_player_position_rect(grid, &pl);
				return_left = check_left_collision(grid, &pl, gravity, 0);
				return_right = check_right_collision(grid, &pl, gravity, 0);
				if (return_left) {
					clear_virus_vertical(grid, &pl, get_left_row(&pl), get_left_column(&pl));
					pl.active = false;
				}
				if (return_right) {
					clear_virus_vertical(grid, &pl, get_right_row(&pl), get_right_column(&pl));
					pl.active = false;
				}
				if (!return_left && !return_right)
				{
					move_player(grid, &pl, gravity, 0, gravity, 0);
					set_player_position_rect(grid, &pl);
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

			/*
			* is_player does not behave as expected(?) when the current row is the top row. (row = GRID_ROWS)
			* This causes check_cell_collision to never report a collision as it checks that flag and the 
			* existence of an entity to determine a collision.
			* To circumvent this I only check if that cell contains an entity.
			* Hope this won't back-fire in the future :)
			*/
			if (check_cell_entity(grid, pl.left.row, pl.left.column) || check_cell_entity(grid, pl.right.row, pl.right.column))
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
			return_left = check_left_collision(grid, &pl, gravity, 0);
			return_right = check_right_collision(grid, &pl, gravity, 0);
			if (return_left) {
				clear_virus_vertical(grid, &pl, get_left_row(&pl), get_left_column(&pl));
				pl.active = false;
			}
			if (return_right) {
				clear_virus_vertical(grid, &pl, get_right_row(&pl), get_right_column(&pl));
				pl.active = false;
			}
			if (!return_left && !return_right)
			{
				move_player(grid, &pl, gravity, 0, gravity, 0);
				set_player_position_rect(grid, &pl);
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