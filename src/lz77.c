/* MyLZ77 -- LZ77 implementation
 * 27/07/2024
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "lz77.h"

static void _emit(List *list, uint16_t pos, uint8_t len, uint8_t byte) {
	listAdd(list, pos >> 8);
	listAdd(list, pos & 0xFF);
	listAdd(list, len);
	listAdd(list, byte);
}

void lz77Compress(const uint8_t *INPUT, uint8_t **output, LZ77Options *opts) {
	List list;
	listInit(&list);

	uint16_t curpos = 0;

	while( INPUT[curpos] != '\0' ) {
		if( curpos == 0 ) {
			_emit(&list, 0, 0, INPUT[curpos++]);
			continue;
		}

		uint16_t pos = 0;
		uint8_t len = 1;
		if( curpos >= opts->searchBufLen ) {
			/* If there's more than searchBufLen bytes behind us, set pos to be
			 * searchBufLen bytes behind us (beginning of search buffer).
			 */
			pos = curpos - opts->searchBufLen;
		}

		const uint16_t OLD_CURPOS = curpos;

		/* Loop through the search buffer */
		for( ; pos < curpos; ++pos ) {
			if( INPUT[pos] == INPUT[curpos] ) {
				for( ; len < opts->lookaheadBufLen; ++len ) {
					if( INPUT[pos + len] != INPUT[++curpos] ) {
						break;
					}
				}

				_emit(&list, OLD_CURPOS - pos, len, INPUT[curpos]);
				goto repeat;
			}
		}

		_emit(&list, 0, 0, INPUT[curpos]);
	repeat:
		++curpos;
	}

	*output = malloc(list.occupied + 1);
	memcpy(*output, list.values, list.occupied);
	(*output)[list.occupied] = '\0';

	listFree(&list);
}

void lz77Decompress(const uint8_t *INPUT, uint8_t **output) {
	List list;
	listInit(&list);

	const uint8_t *curchar = INPUT;
	while( true ) {
		uint16_t pos = (uint16_t)(*curchar++) << 8;
		pos |= *curchar++;

		uint8_t len = *curchar++;
		uint8_t byte = *curchar++;

		if( pos != '\0' && len != '\0' ) {
			uint8_t *ptr = (&list.values[list.occupied]) - pos;

			for( uint16_t i = 0; i < len; ++i ) {
				listAdd(&list, *ptr++);
			}
		}

		if( byte == '\0' ) {
			break;
		}

		listAdd(&list, byte);
	}

	*output = malloc(list.occupied + 1);
	memcpy(*output, list.values, list.occupied);
	(*output)[list.occupied] = '\0';

	listFree(&list);
}

void lz77Print(uint8_t *INPUT) {
	const uint8_t *curchar = INPUT;

	while( true ) {
		uint16_t pos = (uint16_t)(*curchar++) << 8;
		pos |= *curchar++;

		printf("[%hu, ", pos);
		printf("%hhu, ", *curchar++);

		if( *curchar == '\0' ) {
			printf("NULL]\n");
			break;
		}

		printf("%hhu]\n", *curchar++);
	}

	printf("*END*\n");
}
