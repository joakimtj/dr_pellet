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
#define ENTITY_COUNT (GRID_ROWS * GRID_COLUMNS) // The amount of entities in the grid. 

enum {
	LEFT = -1,
	NEUTRAL = 0,
	RIGHT = 1
} typedef direction;

enum {
	EMPTY,
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
} typedef cell;

struct {
	entity_type type_left;
	entity_type type_right;
} typedef pill;

void init_grid(cell grid[GRID_ROWS][GRID_COLUMNS], entity entities[GRID_ROWS][GRID_COLUMNS]) {
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLUMNS; j++) {
			grid[i][j] = (cell){ &entities[i][j] };
		}
	}
}

SDL_Rect create_std_rect(int i, int j) {
	#define POS_X 350
	#define POS_Y 250
	#define RECT_SIZE 30
	#define PADDING 2
	
	SDL_Rect rect = { POS_X + ((PADDING + RECT_SIZE) * j), POS_Y + ((PADDING + RECT_SIZE) * i), RECT_SIZE, RECT_SIZE};
	return rect;
}

int check_adjacent_entities(entity entities[GRID_ROWS][GRID_COLUMNS], int i, int j, entity_type t_type) {
	if (i > 2) 
	{
		if (entities[i-1][j].type == t_type && entities[i-2][j].type == t_type) {
			return 1;
		}
	}	
	return 0;
}

entity create_entity(entity_type e_type, SDL_Rect rect) {
	switch (e_type) {
		case EMPTY:
			return (entity) { EMPTY };
		case RED_VIRUS:
			return (entity) { RED_VIRUS, rect };
		case BLUE_VIRUS:
			return (entity) { BLUE_VIRUS, rect };
		case YELLOW_VIRUS:
			return (entity) { YELLOW_VIRUS, rect };
	}
}

void init_entities(entity entities[GRID_ROWS][GRID_COLUMNS]) {
	#define ENTITY_TOTAL 32 // The amount of entites to be created. Which isn't really correct because we are not adding the GRID_CLEARANCE entities to this. lol
	int v_total = ENTITY_TOTAL;
	int total_empty = 0;
	entity_type e_type = EMPTY;
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLUMNS; j++) 
		{
			if (i > GRID_ROW_CLEARANCE) 
			{
				if (v_total) 
				{	
					SDL_Rect rect = create_std_rect(i, j);
					e_type = (rand() % 4);
					switch (e_type) {
						case EMPTY:
							entities[i][j] = create_entity(e_type, rect);
							total_empty++;
							break;

						case RED_VIRUS:
							if (check_adjacent_entities(entities, i, j, e_type)) {
								entities[i][j] = create_entity(EMPTY, rect);
							}
							else { 
								if (j > 1) {
									if (entities[i][j - 1].type > 0) {
										entities[i][j] = create_entity(EMPTY, rect);
										break;
									}
								}
								entities[i][j] = create_entity(e_type, rect);
								puts("RED_VIRUS");
							}
							break;

						case BLUE_VIRUS:
							if (check_adjacent_entities(entities, i, j, e_type) == 1) {
								entities[i][j] = create_entity(EMPTY, rect);
							}
							else {
								if (j > 1) {
									if (entities[i][j - 1].type > 0) {
										entities[i][j] = create_entity(EMPTY, rect);
										break;
									}
								}
								entities[i][j] = create_entity(e_type, rect);
								puts("BLUE_VIRUS");
							}
							break;

						case YELLOW_VIRUS:
							if (check_adjacent_entities(entities, i, j, e_type)) {
								entities[i][j] = create_entity(EMPTY, rect);
							}
							else {
								if (j > 1) {
									if (entities[i][j - 1].type > 0) {
										entities[i][j] = create_entity(EMPTY, rect);
										break;
									}
								}
								entities[i][j] = create_entity(e_type, rect);
							}
							break;
					}
					v_total--;
				}

			}
			else {
				entities[i][j] = (entity) { EMPTY };
			}
		}
	}
	printf("VIRUS_TOTAL: %d", v_total);
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow(TITLE ,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;
	
	srand(time(NULL));  

	entity entities[GRID_ROWS][GRID_COLUMNS];
	init_entities(entities);
	cell grid[GRID_ROWS][GRID_COLUMNS];
	init_grid(grid, entities);
	
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

	direction dir_x = NEUTRAL;
	int gravity = 1;
	bool has_moved = false;
	pill p1 = { BLUE_VIRUS , YELLOW_VIRUS};
	
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{	
			if (event.key.keysym.sym == SDLK_LEFT) {
				dir_x = LEFT;
			}

			if (event.key.keysym.sym == SDLK_RIGHT) {
				dir_x = RIGHT;
			}

			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		// Logic
		entities[0][0] = create_entity(p1.type_left, create_std_rect(0, 0));
		entities[0][1] = create_entity(p1.type_right, create_std_rect(0, 1));
		grid[0][0].c_entity = &entities[0][0];
		grid[0][1].c_entity = &entities[0][1];
		for (int i = 0; i < GRID_ROWS; i++) 
		{
			for (int j = 0; j < GRID_COLUMNS; j++) 
			{
				if (has_moved) {
					continue;
				}
			}
		}

		// Rendering
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bg_t, NULL, NULL);
		SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);

		for (int i = 0; i < GRID_ROWS; i++) 
		{
			for (int j = 0; j < GRID_COLUMNS; j++) 
			{
				if (grid[i][j].c_entity->type == RED_VIRUS) {
					SDL_RenderCopy(renderer, red_block_t, NULL, &grid[i][j].c_entity->rect);
				}

				if (grid[i][j].c_entity->type == BLUE_VIRUS) {
					SDL_RenderCopy(renderer, blue_block_t, NULL, &grid[i][j].c_entity->rect);
				}
	
				if (grid[i][j].c_entity->type == YELLOW_VIRUS) {
					SDL_RenderCopy(renderer, yellow_block_t, NULL, &grid[i][j].c_entity->rect);
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);
	}
	return 0;
}