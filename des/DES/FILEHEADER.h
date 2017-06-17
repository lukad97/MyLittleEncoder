#ifndef _FILEHEADER_H_
#define _FILEHEADER_H_

#include <stdint.h>
#include <stdlib.h>

#define FILENAME_LEN_MAX 256

typedef struct
{
	int8_t fileName[FILENAME_LEN_MAX];
	uint64_t byteLength;
	// uint8_t crc[4];
	uint32_t crc, pad;
} fileheader_t;


fileheader_t headerCreate(FILE *file, const int8_t *fileName);
int          headerCheck(FILE *file, fileheader_t *header);

void headerPrint(fileheader_t *header);


#endif // _FILEHEADER_H_
