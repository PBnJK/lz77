#ifndef INC_MYLZ77_LZ77_H
#define INC_MYLZ77_LZ77_H

#include <stdint.h>

typedef struct {
	uint8_t searchBufLen;
	uint8_t lookaheadBufLen;
} LZ77Options;

void lz77Compress(const uint8_t *INPUT, uint8_t **output, LZ77Options *opts);
void lz77Decompress(const uint8_t *INPUT, uint8_t **output);

void lz77Print(uint8_t *INPUT);

#endif // !INC_MYLZ77_LZ77_H
