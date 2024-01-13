#include "entity.h"

entity* init_entities(int mode, int n_entities)
{	/*
	*	@param int mode : 0 or 3. Determines which range of types to create. Virus or Block.
	*/
	int RED_COUNT = 0, BLUE_COUNT = 0, YELLOW_COUNT = 0;
	if (mode != 0 && mode != 3) {
		printf("Mode was %d, expected 0 or 3.+\n", mode);
		return -1;
	}
	entity* entities = malloc(sizeof(entity) * n_entities);
	entity_type e_type;
	for (int i = 0; i < n_entities; i++)
	{
		e_type = (rand() % 3 + mode);
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