#ifndef RENDER_H
#define RENDER_H

#include <SDL.h>
#include "config.h"
#include "util.h"
#include "grid.h"

void render_character_area(SDL_Renderer* renderer, SDL_Texture* dr_pellet_t);

void render_grid_area(SDL_Renderer* renderer);

void render_bg(SDL_Renderer* renderer);

#endif