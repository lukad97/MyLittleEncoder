#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "aes_file_handler.h"
#include "aes.h"
#include "..\file_header\file_header.h"

#define BLOCK_SIZE 16

// -------------- FILE ENCRYPTION ---------------------

int encryptFileECB(char *name, uc* key)
{
	FILE *in, *out;
	fileheader_t head;
	char outName[256];
	uc state[BLOCK_SIZE];
	int i = 0;

    uc roundKeys[11][16];
    getRoundKeys(key, roundKeys);

	in = fopen(name, "rb");
	FILE_CHECK(in);

	strcpy(outName, name);
	strcat(outName, ".dat");

	out = fopen(outName, "wb");
	FILE_CHECK(out);

	head = headerCreate(in, get_filename_from_path(name));

    uc *p = (uc*) &head;

	for (int i = 0; i < sizeof(head)/BLOCK_SIZE; i++) {
        encryptBlockRoundKeys(p, roundKeys);
        fwrite(p, sizeof(uc), BLOCK_SIZE, out);
        p += BLOCK_SIZE;
	}

    while ((i = fread(state, sizeof(uc), BLOCK_SIZE, in)))
    {
        for (; i<BLOCK_SIZE; i++)
            state[i] = 0;
        encryptBlockRoundKeys(state, roundKeys);
        fwrite(state, sizeof(uc), BLOCK_SIZE, out);
    }

	fclose(in);
	fclose(out);
	return 0;
}

int decryptFileECB(char *name, uc* key)
{
	FILE *in, *out;
	fileheader_t header;
	char outName[256];
	uc state[BLOCK_SIZE];
	int i = 0;

    uc invRoundKeys[11][16];
    getInvRoundKeys(key, invRoundKeys);

	in = fopen(name, "rb");
	FILE_CHECK(in);

	uc *p = (uc*) &header;
	for (int i = 0; i < sizeof(header)/BLOCK_SIZE; i++) {
	    fread(p, sizeof(uc), BLOCK_SIZE, in);
        decryptBlockRoundKeys(p, invRoundKeys);
        p += BLOCK_SIZE;
	}

    uint64_t len = header.byteLength;

    strcpy(outName, (char*) header.fileName);
    out = fopen(outName, "rb");
    if (out != NULL) {
        fclose(out);
        outName[0] = '\0';
        srand(time(NULL));
        sprintf(outName, "%d\0", rand());
        strcat(outName, (char*) header.fileName);
    }

	out = fopen(outName, "wb");
	FILE_CHECK(out);

	while ((i = fread(state, sizeof(uc), BLOCK_SIZE, in)))
    {
        decryptBlockRoundKeys(state, invRoundKeys);
        fwrite(state, sizeof(uc), len > BLOCK_SIZE ? BLOCK_SIZE : len, out);
        len -= BLOCK_SIZE;
    }
	fclose(in);
	fclose(out);

	in = fopen(outName, "rb");
	FILE_CHECK(in);

    i = headerCheck(in, &header);

    fclose(in);

	return i;
}
