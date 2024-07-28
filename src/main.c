/* MyLZ77 -- Entry-point
 * 27/07/2024
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lz77.h"

static uint8_t *_readFile(const char *PATH) {
	FILE *file = fopen(PATH, "rb");
	if( file == NULL ) {
		printf("couldn't open '%s'", PATH);
		exit(EXIT_FAILURE);
	}

	fseek(file, 0L, SEEK_END);
	const size_t FILE_SIZE = ftell(file);
	rewind(file);

	uint8_t *buffer = malloc(FILE_SIZE + 1);
	if( buffer == NULL ) {
		printf("not enough memory for opening '%s'", PATH);
		exit(EXIT_FAILURE);
	}

	const size_t BYTES_READ = fread(buffer, sizeof(char), FILE_SIZE, file);
	if( BYTES_READ < FILE_SIZE ) {
		printf("not enough memory for opening '%s'", PATH);
		exit(EXIT_FAILURE);
	}

	fclose(file);
	return buffer;
}

static void _usage(void) {
	printf("usage: my_lz77 [-c, -d, -a {n} {m}, -o {file}] [-i {file}| "
		   "{string}]\n");
	printf("===\n");
	printf("-c: compress mode (default)\n");
	printf("-d: decompress mode\n");
	printf("-a: define search and look-ahead buffer size, respectively n "
		   "and m.\n"
		   "    This option is unused when decompressing.\n\n");
	printf("-o: write result to file\n");
	printf("-i: use this file's contents as input\n");
	printf("string: use this string's content as input\n");
}

int main(int argc, char *argv[]) {
	LZ77Options opts = { 6, 6 };

	uint8_t *output = NULL;
	uint8_t *input = NULL;

	bool decompress = false;
	bool writef = false;
	bool readf = false;

	char *outf = NULL;

	if( argc < 2 ) {
		_usage();
		exit(EXIT_FAILURE);
	}

	while( true ) {
		argv++;

		if( *argv && **argv == '-' ) {
			++*argv;
			switch( **argv ) {
			case 'd':
				decompress = true;
			case 'c': /* fallthrough */
				break;
			case 'a':
				argv++;

				opts.searchBufLen = strtol(*argv, NULL, 0);
				argv++;

				opts.lookaheadBufLen = strtol(*argv, NULL, 0);
				argv++;
			case 'o':
				writef = true;

				argv++;
				outf = *argv;
				break;
			case 'i':
				readf = true;
				argv++;
				input = _readFile(*argv);
				break;
			default:
				printf("invalid option -%c\n\n", **argv);
				_usage();
				exit(EXIT_FAILURE);
			}
		} else {
			if( !input && !*argv ) {
				printf("no input provided!\n\n");
				_usage();
				exit(EXIT_FAILURE);
			}

			if( decompress ) {
				lz77Decompress(readf ? input : (const uint8_t *)*argv, &output);
			} else {
				lz77Compress(
					readf ? input : (const uint8_t *)*argv, &output, &opts);
			}

			break;
		}
	}

	if( output == NULL ) {
		printf("no output!\n");
		exit(EXIT_FAILURE);
	}

	if( writef ) {
		FILE *file = fopen(outf, "wb");

		while( true ) {
			fputc(*output++, file);
			fputc(*output++, file);
			fputc(*output++, file);
			fputc(*output, file);

			if( *output++ == '\0' ) {
				break;
			}
		}

		fclose(file);
	} else {
		printf("%s\n", output);
	}

	if( readf ) {
		free(input);
	}

	return EXIT_SUCCESS;
}
