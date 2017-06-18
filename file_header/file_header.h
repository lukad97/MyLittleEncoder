#ifndef _FILE_HEADER_H_
#define _FILE_HEADER_H_

#include <stdint.h>
#include <stdlib.h>

#define FILENAME_LEN_MAX 256

#define ALLOC_CHECK(p)  if (!(p)) return(1)
#define FILE_CHECK(p)   if (!(p)) return(2)

#define ALLOC_ERR 1
#define FILE_ERR  2

typedef struct
{
	int8_t fileName[FILENAME_LEN_MAX];
	uint64_t byteLength;
	uint32_t crc, pad;
} fileheader_t;


fileheader_t headerCreate(FILE *file, const char *fileName);
int          headerCheck(FILE *file, fileheader_t *header);

void headerPrint(fileheader_t *header);

#endif // _FILE_HEADER_H_
