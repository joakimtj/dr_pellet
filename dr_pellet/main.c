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
#define GRID_ROW_CLEARANCE 4 // Viruses will not spawn above or on this ROW.
#define ENTITY_N 66 // The N viruses to be spawned on the grid.
#define RECT_SIZE 30
#define PADDING 1

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
	int pos_x[2];
	int pos_y[2];
	bool has_moved;
} typedef pill;

int get_random_integer(int i) {
	return rand() % i;
}

SDL_Rect* get_rect(cell c) {
	return &c.c_entity->rect;
}

void set_grid_position_rect(SDL_Rect* rect, int i, int j) {
	rect->x = (WINDOW_WIDTH / 2) - (GRID_ROWS * RECT_SIZE) + (PADDING + RECT_SIZE) * j;
	rect->y = (WINDOW_HEIGHT / 4) + (PADDING + RECT_SIZE) * i;
	rect->w = RECT_SIZE;
	rect->h = RECT_SIZE;
}

void init_grid(cell grid[GRID_ROWS][GRID_COLUMNS], entity* entities) {
	int entity_count = 0;
	int random_i;
	for (int i = 0; i < GRID_ROWS; i++)
	{
		for (int j = 0; j < GRID_COLUMNS; j++)
		{	
			if (i > GRID_ROW_CLEARANCE) 
			{
				if (entity_count < ENTITY_N)
				{
					random_i = get_random_integer(10);
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
			
		}
	}
	printf("entity count: %d\n", entity_count);
}



SDL_Rect create_rect() {
	#define DEFAULT_POS 0	
	SDL_Rect rect = { DEFAULT_POS + (PADDING + RECT_SIZE), DEFAULT_POS + (PADDING + RECT_SIZE), RECT_SIZE, RECT_SIZE};
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
	return;
}
entity* init_entities() {
	int RED_COUNT = 0, BLUE_COUNT = 0, YELLOW_COUNT = 0;
	entity* entities = malloc(sizeof(entity) * ENTITY_N);
	entity_type e_type;
	for (int i = 0; i < ENTITY_N; i++)
	{
		e_type = (rand() % 3) + 1;
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

void render_grid_area(SDL_Renderer* renderer) {
	SDL_Rect bg_rect;
	bg_rect.x = WINDOW_WIDTH / 5;
	bg_rect.y = WINDOW_HEIGHT / 4;
	bg_rect.w = GRID_COLUMNS * RECT_SIZE + PADDING + 6;
	bg_rect.h = GRID_ROWS * RECT_SIZE + PADDING + 10;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0xd5, 0xab, 0xff, 200);
	SDL_RenderFillRect(renderer, &bg_rect);
	SDL_RenderDrawRect(renderer, &bg_rect);

	// White Outline
	SDL_Rect bg_rect1;
	bg_rect1.x = -1 + WINDOW_WIDTH / 5;
	bg_rect1.y = -1 + WINDOW_HEIGHT / 4;
	bg_rect1.w = GRID_COLUMNS * RECT_SIZE + PADDING + 6 + 2;
	bg_rect1.h = GRID_ROWS * RECT_SIZE + PADDING + 10 + 2;

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 255);
	SDL_RenderDrawRect(renderer, &bg_rect1);

}

void render_bg(SDL_Renderer* renderer) {
	for (int i = 0; i <= WINDOW_HEIGHT / RECT_SIZE; i++)
	{
		for (int j = 0; j <= WINDOW_WIDTH / RECT_SIZE; j++)
		{
			SDL_Rect bg_rect = create_rect();
			bg_rect.x = -16 + RECT_SIZE * j;
			bg_rect.y = -2 + RECT_SIZE * i;
			bg_rect.w = RECT_SIZE;
			bg_rect.h = RECT_SIZE;

			if (j % 2 == 0) {

				SDL_SetRenderDrawColor(renderer, 0x8b, 0xb4, 0xd9, 255);
				SDL_RenderFillRect(renderer, &bg_rect);
				SDL_RenderDrawRect(renderer, &bg_rect);
			}
			else if (i % 2 == 0) {
				SDL_SetRenderDrawColor(renderer, 0x8b, 0xb4, 0xd9, 255);
				SDL_RenderFillRect(renderer, &bg_rect);
				SDL_RenderDrawRect(renderer, &bg_rect);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0x8b, 0xd9, 0xd2, 255);
				SDL_RenderFillRect(renderer, &bg_rect);
				SDL_RenderDrawRect(renderer, &bg_rect);
			}
		}
	}
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow(TITLE ,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;
	
	srand(time(NULL));  

	entity* entities = init_entities();
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
	pill p1 = { BLUE_VIRUS , YELLOW_VIRUS};
	p1.has_moved = false;
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

		// Rendering
		SDL_RenderClear(renderer);
		
		render_bg(renderer);

		render_grid_area(renderer);

		for (int i = 0; i < GRID_ROWS; i++) 
		{
			for (int j = 0; j < GRID_COLUMNS; j++) 
			{
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

	return 0;
}