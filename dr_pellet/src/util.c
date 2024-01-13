#include "util.h"

int get_random_integer(int i)
{
	return rand() % i;
}

SDL_Rect create_rect()
{
	#define DEFAULT_POS 0
	SDL_Rect rect = { DEFAULT_POS + (PADDING + RECT_SIZE), DEFAULT_POS + (PADDING + RECT_SIZE), RECT_SIZE, RECT_SIZE };
	return rect;
}