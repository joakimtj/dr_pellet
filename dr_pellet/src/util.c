#include "util.h"

int get_random_integer(int i)
{
	return rand() % i;
}

SDL_Rect create_rect()
{
	SDL_Rect rect = { 0, 0, RECT_SIZE, RECT_SIZE };
	return rect;
}