#include "render.h"

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