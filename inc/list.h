#ifndef INC_MYLZ77_LIST_H
#define INC_MYLZ77_LIST_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t size;
	size_t occupied;
	uint8_t *values;
} List;

void listInit(List *list);
void listFree(List *list);

void listAdd(List *list, uint8_t value);

#endif // !INC_MYLZ77_LIST_H
