/* MyLZ77 -- Small dynamic array implementation
 * 27/07/2024
 */

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

static void _grow(List *list, const size_t TO) {
	list->size = TO;

	list->values = realloc(list->values, TO);
	if( list->values == NULL ) {
		printf("malloc failed (oh god)\n");
		exit(EXIT_FAILURE);
	}
}

void listInit(List *list) {
	list->size = 0;
	list->occupied = 0;
	list->values = NULL;
}

void listFree(List *list) {
	free(list->values);
	listInit(list);
}

void listAdd(List *list, uint8_t value) {
	if( list->size <= list->occupied ) {
		_grow(list, list->size < 8 ? 8 : list->size * 2);
	}

	list->values[list->occupied++] = value;
}
