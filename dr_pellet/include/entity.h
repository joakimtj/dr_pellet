#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <SDL_rect.h>
#include "util.h"

typedef enum {
	RED_VIRUS,
	BLUE_VIRUS,
	YELLOW_VIRUS,
	RED_BLOCK,
	BLUE_BLOCK,
	YELLOW_BLOCK
} entity_type;

struct {
	entity_type type;
	SDL_Rect rect;
} typedef entity;

entity* init_entities(int mode, int n_entities);

#endif