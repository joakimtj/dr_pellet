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

typedef enum game_state {
	RUNNING,
	PAUSED,
} game_state;

int main(int argc, char** argv)
{
	srand(time(NULL));

	SDL_Init(SDL_INIT_VIDEO);

	int numRenderDrivers = SDL_GetNumRenderDrivers();
	printf("Available Render Drivers:\n");
	for (int i = 0; i < numRenderDrivers; i++) {
		SDL_RendererInfo info;
		SDL_GetRenderDriverInfo(i, &info);
		printf("%d: %s\n", i, info.name);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* icon_bmp = SDL_LoadBMP("assets/icon.bmp");
	SDL_SetWindowIcon(window, icon_bmp);
	SDL_FreeSurface(icon_bmp);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_Texture** texture_a = init_textures(renderer, 2);

	SDL_Event event;

	entity* entities_v = init_entities(0, VIRUS_N);
	entity* entities_p = init_entities(3, PILL_N * 2);

	cell grid[GRID_ROWS][GRID_COLUMNS];
	init_grid(grid, entities_v);

	pill pl = init_pill(entities_p);
	set_initial_pill(grid, &pl);
	int pill_count = 2;

	uint32_t previous_time = SDL_GetTicks();
	uint32_t current_time = 0.0f;
	float delta_time = 0.0f;

	uint32_t frame_count = 0;
	uint32_t last_second = previous_time;

	SDL_Rect bar = { 402, 200, 50, 50 };

	float width = 0;

	float step_time = 0;
	float step_limit = 1;

	float grace_period = 0;
	float interp_factor = 0;

	int gravity = 1;
	
	game_state state = RUNNING;

	int ret = 0;

	bool quit = false;
	while (!quit)
	{

		current_time = SDL_GetTicks();
		delta_time = (current_time - previous_time) / 1000.0f;
		previous_time = current_time;

		if (!get_pill_active(&pl))
			width += (delta_time * (100.0f / 3.f));

		if (get_pill_active(&pl))
			step_time += delta_time;

		if (step_time) interp_factor = step_time / step_limit; // Goes unused but it remains here.

		if (!get_pill_active(&pl)) grace_period += delta_time;

		frame_count++;
		if (current_time - last_second >= 1000) {
			printf("FPS: %d\n", frame_count);
			frame_count = 0;
			last_second = current_time;
		}

		while (SDL_PollEvent(&event))
		{	
			if (event.key.keysym.sym == SDLK_e && event.key.state == SDL_PRESSED && get_pill_active(&pl) && state == RUNNING) {
				if (get_rotation_state(&pl) > 3) set_rotation_state(&pl, 0);
				ret = rotate_player_grid(grid, &pl, false);
				if (ret != 0) {
					puts("Can't move.");
				}
				else {
					set_player_position_rect(grid, &pl);
					set_rotation_state(&pl, get_rotation_state(&pl) + 1);
				}
			}

			if (event.key.keysym.sym == SDLK_r && event.key.state == SDL_PRESSED && get_pill_active(&pl) && state == RUNNING) {
				set_rotation_state(&pl, get_rotation_state(&pl) - 1);
				if (get_rotation_state(&pl) < 0) set_rotation_state(&pl, 3);
				ret = rotate_player_grid(grid, &pl, true);
				if (ret != 0) {
					puts("Can't move.");
					set_rotation_state(&pl, get_rotation_state(&pl) + 1);
				}
				else {
					set_player_position_rect(grid, &pl);
				}
			}

			if (event.key.keysym.sym == SDLK_s && event.key.state == SDL_PRESSED && get_pill_active(&pl) && state == RUNNING)
			{
				step_time = 0;
				set_player_position_rect(grid, &pl);
				update_player_grid(grid, &pl, gravity);
			}

			if (event.key.keysym.sym == SDLK_a && event.key.state == SDL_PRESSED && get_pill_active(&pl) && state == RUNNING) {
				if (!(check_cell_collision(grid, &pl, 0, LEFT, 0, LEFT)))
				{
					if (get_left_column(&pl) + LEFT >= 0 && get_right_column(&pl) + LEFT >= 0)
					{
						move_player_grid(grid, &pl, 0, LEFT, 0, LEFT);
						set_player_position_rect(grid, &pl);
					}
				}
			}

			if (event.key.keysym.sym == SDLK_d && event.key.state == SDL_PRESSED && get_pill_active(&pl) && state == RUNNING) {
				if (!(check_cell_collision(grid, &pl, 0, RIGHT, 0, RIGHT)))
				{
					if (get_left_column(&pl) + RIGHT < GRID_COLUMNS && get_right_column(&pl) + RIGHT < GRID_COLUMNS)
					{
						move_player_grid(grid, &pl, 0, RIGHT, 0, RIGHT);
						set_player_position_rect(grid, &pl);
					}
				}
			}

			if (event.key.keysym.sym == SDLK_ESCAPE && event.key.state == SDL_PRESSED) {
				if (state == PAUSED) state = RUNNING;
				else state = PAUSED;
			}

			if (event.key.keysym.sym == SDLK_g && event.key.state == SDL_PRESSED) {
				
			}

			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		switch (state)
		{
			case RUNNING:
				// Activate a new pill
				if (!get_pill_active(&pl))
				{
					grace_period = 0;

					pill_count += 2;

					disable_pill(grid, &pl);

					int difference_left = calculate_difference_left(&pl);
					int difference_right = calculate_difference_right(&pl);

					set_left_row(&pl, difference_left);
					set_right_row(&pl, difference_right);

					set_left_entity(&pl, entities_p, pill_count);
					set_right_entity(&pl, entities_p, pill_count + 1);

					if (check_cell_entity(grid, pl.left.row, pl.left.column) || check_cell_entity(grid, pl.right.row, pl.right.column)) {
						quit = true;
						printf("Oh no... ! You lost !\n");
					}
					else {
						move_player_grid(grid, &pl, 0, 0, 0, 0);
						set_player_position_rect(grid, &pl);
						set_pill_active(&pl, true);
					}
				}

				// Step
				if (step_time > step_limit && get_pill_active(&pl)) {
					step_time = 0;
					set_player_position_rect(grid, &pl);
					update_player_grid(grid, &pl, gravity);
				}
				break;

			case PAUSED:
				break;
		}
		

		// Rendering
		SDL_RenderClear(renderer);
		
		render_bg(renderer);

		render_grid_area(renderer);

		render_character_area(renderer, texture_a[1]);

		// render_grid_edge(renderer, texture_a[2]);

		render_grid_entities(renderer, grid, texture_a[0]);

		render_player(renderer, &pl, texture_a[0]);

		if (!get_pill_active(&pl))
		{	
			render_grace(renderer, &bar, width, delta_time);
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