#ifndef UTIL_H
#define UTIL_H

#include <SDL_rect.h>
#include <stdlib.h>
#include "grid.h"

int get_random_integer(int i);

SDL_Rect* get_rect(cell c);

SDL_Rect create_rect();

#endif