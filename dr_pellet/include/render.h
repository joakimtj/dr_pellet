#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "config.h"
#include "util.h"
#include "grid.h"

SDL_Texture** init_textures(SDL_Renderer* renderer, int allocated);

void render_character_area(SDL_Renderer* renderer, SDL_Texture* dr_pellet_t);

void render_grid_area(SDL_Renderer* renderer);

void render_grid_entities(SDL_Renderer* renderer, cell grid[GRID_ROWS][GRID_COLUMNS], SDL_Texture* entity_sheet_t);

void render_player(SDL_Renderer* renderer, pill* pl, SDL_Texture* entity_sheet_t);

void render_grace(SDL_Renderer* renderer, SDL_Rect* bar, float width, float delta_time);

void render_bg(SDL_Renderer* renderer);

#endif // RENDER_H