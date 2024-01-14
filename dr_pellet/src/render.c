#include "render.h"

SDL_Texture** init_textures(SDL_Renderer* renderer, int allocated)
{
	if (allocated < 0) {
		printf("That is not a valid value for allocated.\n");
		return -1;
	}
	SDL_Surface* entity_sheet_bmp = SDL_LoadBMP("assets/entity_sheet.bmp");
	SDL_Texture* entity_sheet_t = SDL_CreateTextureFromSurface(renderer, entity_sheet_bmp);
	SDL_FreeSurface(entity_sheet_bmp);

	SDL_Surface* dr_pellet_bmp = SDL_LoadBMP("assets/dr_pellet.bmp");
	SDL_Texture* dr_pellet_t = SDL_CreateTextureFromSurface(renderer, dr_pellet_bmp);
	SDL_FreeSurface(dr_pellet_bmp);

	SDL_Surface* grid_bg_sheet_bmp = SDL_LoadBMP("assets/grid_bg_sheet.bmp");
	SDL_Texture* grid_bg_sheet_t = SDL_CreateTextureFromSurface(renderer, grid_bg_sheet_bmp);
	SDL_FreeSurface(grid_bg_sheet_bmp);

	SDL_Texture** texture_a = malloc(sizeof(SDL_Texture*) * allocated);
	if (texture_a == NULL) {
		fprintf(stderr, "Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	texture_a[0] = entity_sheet_t;
	texture_a[1] = dr_pellet_t;
	texture_a[2] = grid_bg_sheet_t;
	return texture_a;
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
	// Unused.
	SDL_Rect grid_rect;
	grid_rect.x = WINDOW_WIDTH / 5;
	grid_rect.y = WINDOW_HEIGHT / 4;
	grid_rect.w = GRID_COLUMNS * RECT_SIZE + 6; // Magic number to fit the rect neatly behind the entities.
	grid_rect.h = GRID_ROWS * RECT_SIZE + 10;

	SDL_SetRenderDrawColor(renderer, 0x00, 0x0, 0x00, 200);
	SDL_RenderFillRect(renderer, &grid_rect);
	SDL_RenderDrawRect(renderer, &grid_rect);

	// White Outline
	SDL_Rect outline_rect;
	outline_rect.x = -1 + WINDOW_WIDTH / 5;
	outline_rect.y = -1 + WINDOW_HEIGHT / 4;
	outline_rect.w = GRID_COLUMNS * RECT_SIZE + 6 + 2;
	outline_rect.h = GRID_ROWS * RECT_SIZE + 10 + 2;

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 255);
	SDL_RenderDrawRect(renderer, &outline_rect);
}

void render_grid_edge(SDL_Renderer* renderer, SDL_Texture* grid_bg_sheet_t)
{
	SDL_Rect rect = (SDL_Rect) {0, 0, 30, 30};

	SDL_Rect rect_a[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
		{
			rect_a[i][j] = (SDL_Rect){ 30 * j, 30 * i, 30, 30 };
		}
	}

	for (int i = 0; i < GRID_ROWS + 2; i++)
	{
		for (int j = 0; j < GRID_COLUMNS + 2; j++)
		{
			set_grid_position_rect(&rect, i, j, -30);

			if (i == 0 && j == 0) SDL_RenderCopy(renderer, grid_bg_sheet_t, &rect_a[0][0], &rect);
			else if (i == 0 && j == GRID_COLUMNS + 1) SDL_RenderCopy(renderer, grid_bg_sheet_t, &rect_a[0][2], &rect);

			else if (i == 0 && (j > 0 && j < GRID_COLUMNS + 1)) SDL_RenderCopy(renderer, grid_bg_sheet_t, &rect_a[0][1], &rect);
			else if (i == GRID_ROWS + 1 && j == 0) SDL_RenderCopy(renderer, grid_bg_sheet_t, &rect_a[2][0], &rect);
			else if (i == GRID_ROWS + 1 && j == GRID_COLUMNS + 1) SDL_RenderCopy(renderer, grid_bg_sheet_t, &rect_a[2][2], &rect);

			else if ((i > 0 || i < GRID_ROWS + 1) && j == 0 && i != 0) SDL_RenderCopy(renderer, grid_bg_sheet_t, &rect_a[1][0], &rect);
			else if ((i > 0 || i < GRID_ROWS + 1) && j == GRID_COLUMNS + 1) SDL_RenderCopy(renderer, grid_bg_sheet_t, &rect_a[1][2], &rect);

			else if (i == GRID_ROWS + 1 && (j > 0 && j < GRID_COLUMNS + 1)) SDL_RenderCopy(renderer, grid_bg_sheet_t, &rect_a[2][1], &rect);
			else if ((i > 0 || i < GRID_ROWS + 1) && (j > 0 && j < GRID_COLUMNS + 1)) SDL_RenderCopy(renderer, grid_bg_sheet_t, &rect_a[1][1], &rect);
		}
	}
}

void render_grid_entities(SDL_Renderer* renderer, cell grid[GRID_ROWS][GRID_COLUMNS], SDL_Texture* entity_sheet_t)
{
	SDL_Rect red_block = { 0, 30, RECT_SIZE, RECT_SIZE };
	SDL_Rect blue_block = { 30, 30, RECT_SIZE, RECT_SIZE };
	SDL_Rect yellow_block = { 60, 30, RECT_SIZE, RECT_SIZE };
	SDL_Rect red_virus = { 0, 0, RECT_SIZE, RECT_SIZE };
	SDL_Rect blue_virus = { 30, 0, RECT_SIZE, RECT_SIZE };
	SDL_Rect yellow_virus = { 60, 0, RECT_SIZE, RECT_SIZE };

	for (int i = 0; i < GRID_ROWS; i++)
	{
		for (int j = 0; j < GRID_COLUMNS; j++)
		{
			if (check_cell_entity(grid, i, j) && !grid[i][j].is_player)
			{
				switch (get_cell_type(grid, i, j))
				{
				case RED_BLOCK:
					SDL_RenderCopy(renderer, entity_sheet_t, &red_block, get_rect(grid[i][j]));
					break;

				case BLUE_BLOCK:
					SDL_RenderCopy(renderer, entity_sheet_t, &blue_block, get_rect(grid[i][j]));
					break;

				case YELLOW_BLOCK:
					SDL_RenderCopy(renderer, entity_sheet_t, &yellow_block, get_rect(grid[i][j]));
					break;

				case RED_VIRUS:
					SDL_RenderCopy(renderer, entity_sheet_t, &red_virus, get_rect(grid[i][j]));
					break;

				case BLUE_VIRUS:
					SDL_RenderCopy(renderer, entity_sheet_t, &blue_virus, get_rect(grid[i][j]));
					break;

				case YELLOW_VIRUS:
					SDL_RenderCopy(renderer, entity_sheet_t, &yellow_virus, get_rect(grid[i][j]));
					break;
				}
			}
		}
	}
}

void render_player(SDL_Renderer* renderer, pill* pl, SDL_Texture* entity_sheet_t)
{
	SDL_Rect red_block = { 0, 30, RECT_SIZE, RECT_SIZE };
	SDL_Rect blue_block = { 30, 30, RECT_SIZE, RECT_SIZE };
	SDL_Rect yellow_block = { 60, 30, RECT_SIZE, RECT_SIZE };

	switch (pl->left.h_entity->type)
	{
	case RED_BLOCK:
		SDL_RenderCopy(renderer, entity_sheet_t, &red_block, &pl->left.h_entity->rect);
		break;

	case BLUE_BLOCK:
		SDL_RenderCopy(renderer, entity_sheet_t, &blue_block, &pl->left.h_entity->rect);
		break;

	case YELLOW_BLOCK:
		SDL_RenderCopy(renderer, entity_sheet_t, &yellow_block, &pl->left.h_entity->rect);
		break;
	}

	switch (pl->right.h_entity->type)
	{
	case RED_BLOCK:
		SDL_RenderCopy(renderer, entity_sheet_t, &red_block, &pl->right.h_entity->rect);
		break;

	case BLUE_BLOCK:
		SDL_RenderCopy(renderer, entity_sheet_t, &blue_block, &pl->right.h_entity->rect);
		break;

	case YELLOW_BLOCK:
		SDL_RenderCopy(renderer, entity_sheet_t, &yellow_block, &pl->right.h_entity->rect);
		break;
	}
}

void render_grace(SDL_Renderer* renderer, SDL_Rect* bar, float width, float delta_time)
{	
	if ((int)width > 100) {
		width = 100;
	}
	bar->w = (int) width;
	if (bar->w < 2500)
		SDL_SetRenderDrawColor(renderer, 0x8b, 0xff, 0x3d, 255);
	else SDL_SetRenderDrawColor(renderer, 0xff, 0x2a, 0x3d, 255);
	SDL_RenderFillRect(renderer, bar);
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